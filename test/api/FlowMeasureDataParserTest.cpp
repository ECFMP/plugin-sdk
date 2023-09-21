#include "api/FlowMeasureDataParser.h"
#include "ECFMP/SdkEvents.h"
#include "ECFMP/flowmeasure/CustomFlowMeasureFilter.h"
#include "ECFMP/flowmeasure/FlowMeasure.h"
#include "ECFMP/flowmeasure/Measure.h"
#include "ECFMP/flowmeasure/MultipleLevelFilter.h"
#include "ECFMP/flowmeasure/RangeToDestinationFilter.h"
#include "api/FlowMeasureFilterParserInterface.h"
#include "api/FlowMeasureMeasureParserInterface.h"
#include "api/InternalApiElementCollection.h"
#include "api/InternalElementCollectionTypes.h"
#include "date/ParseDateStrings.h"
#include "event/ConcreteEvent.h"
#include "eventbus/InternalEventBusFactory.h"
#include "flightinformationregion/ConcreteFlightInformationRegion.h"
#include "flowmeasure/ConcreteAirportFilter.h"
#include "flowmeasure/ConcreteFlowMeasureFilters.h"
#include "flowmeasure/ConcreteMeasure.h"
#include "mock/MockEuroscopeAircraftFactory.h"
#include "mock/MockLogger.h"
#include "nlohmann/json.hpp"
#include "plugin/InternalSdkEvents.h"

namespace ECFMPTest::Api {

    // Declared outside the test for constantness
    const std::chrono::system_clock::time_point now =
            std::chrono::floor<std::chrono::seconds>(std::chrono::system_clock::now());
    const std::chrono::system_clock::time_point plusOneHour = now + std::chrono::hours(1);
    const std::chrono::system_clock::time_point plusTwoHours = now + std::chrono::hours(2);
    const std::chrono::system_clock::time_point minusOneHour = now - std::chrono::hours(1);
    const std::chrono::system_clock::time_point minusTwoHours = now - std::chrono::hours(2);

    class MockFlowMeasureFilterParser : public ECFMP::Api::FlowMeasureFilterParserInterface
    {
        public:
        MOCK_METHOD(
                std::unique_ptr<ECFMP::FlowMeasure::FlowMeasureFilters>, Parse,
                (const nlohmann::json& data, const ECFMP::Api::InternalEventCollection& events), (const, override)
        );
    };

    class MockFlowMeasureMeasureParser : public ECFMP::Api::FlowMeasureMeasureParserInterface
    {
        public:
        MOCK_METHOD(
                std::unique_ptr<ECFMP::FlowMeasure::Measure>, Parse, (const nlohmann::json& data), (const, override)
        );
    };

    class MockFlowMeasuresUpdatedEventHandler
        : public ECFMP::EventBus::EventListener<ECFMP::Plugin::FlowMeasuresUpdatedEvent>
    {
        public:
        explicit MockFlowMeasuresUpdatedEventHandler(int expectedItemCount) : expectedItemCount(expectedItemCount)
        {}

        void OnEvent(const ECFMP::Plugin::FlowMeasuresUpdatedEvent& event) override
        {
            callCount++;
            EXPECT_EQ(expectedItemCount, event.flowMeasures->Count());
        }

        [[nodiscard]] auto GetCallCount() const -> int
        {
            return callCount;
        }

        private:
        int expectedItemCount;

        int callCount = 0;
    };

    class MockInternalFlowMeasuresUpdatedEventHandler
        : public ECFMP::EventBus::EventListener<ECFMP::Plugin::InternalFlowMeasuresUpdatedEvent>
    {
        public:
        explicit MockInternalFlowMeasuresUpdatedEventHandler(int expectedItemCount)
            : expectedItemCount(expectedItemCount)
        {}

        void OnEvent(const ECFMP::Plugin::InternalFlowMeasuresUpdatedEvent& event) override
        {
            callCount++;
            EXPECT_EQ(expectedItemCount, event.flowMeasures->Count());
        }

        [[nodiscard]] auto GetCallCount() const -> int
        {
            return callCount;
        }

        private:
        int expectedItemCount;

        int callCount = 0;
    };

    struct FlowMeasureDataParserTestCase {
        std::string description;
        nlohmann::json data;
        int expectedId;
        int expectedEventId;
        std::string expectedIdentifier;
        std::string expectedReason;
        std::chrono::system_clock::time_point expectedStart;
        std::chrono::system_clock::time_point expectedEnd;
        std::chrono::system_clock::time_point expectedWithdrawnAt;
        ECFMP::FlowMeasure::MeasureStatus expectedStatus;
        std::list<int> expectedNotifiedFirIds;
    };

    class FlowMeasureDataParserTest : public testing::TestWithParam<FlowMeasureDataParserTestCase>
    {
        public:
        void SetUp() override
        {
            // Create the mock parsers
            auto filterParser = std::make_unique<MockFlowMeasureFilterParser>();
            filterParserRaw = filterParser.get();
            auto measureParser = std::make_unique<MockFlowMeasureMeasureParser>();
            measureParserRaw = measureParser.get();

            eventBus = ECFMP::EventBus::MakeEventBus();
            mockEventHandler = std::make_shared<MockFlowMeasuresUpdatedEventHandler>(1);
            mockEventHandlerInternal = std::make_shared<MockInternalFlowMeasuresUpdatedEventHandler>(1);
            eventBus->SubscribeSync<ECFMP::Plugin::FlowMeasuresUpdatedEvent>(mockEventHandler);
            eventBus->SubscribeSync<ECFMP::Plugin::InternalFlowMeasuresUpdatedEvent>(mockEventHandlerInternal);

            customFilters =
                    std::make_shared<std::vector<std::shared_ptr<ECFMP::FlowMeasure::CustomFlowMeasureFilter>>>();
            parser = std::make_unique<ECFMP::Api::FlowMeasureDataParser>(
                    std::move(filterParser), std::move(measureParser), std::make_shared<Log::MockLogger>(), eventBus,
                    customFilters
            );

            firs.Add(std::make_shared<ECFMP::FlightInformationRegion::ConcreteFlightInformationRegion>(
                    1, "EGTT", "London"
            ));
            firs.Add(std::make_shared<ECFMP::FlightInformationRegion::ConcreteFlightInformationRegion>(
                    2, "EGPX", "Scottish"
            ));

            events.Add(std::make_shared<ECFMP::Event::ConcreteEvent>(
                    1, "Test event", now, plusOneHour, firs.Get(1), "abc",
                    std::vector<std::shared_ptr<ECFMP::Event::EventParticipant>>{}
            ));
        }

        std::shared_ptr<std::vector<std::shared_ptr<ECFMP::FlowMeasure::CustomFlowMeasureFilter>>> customFilters;
        std::shared_ptr<MockFlowMeasuresUpdatedEventHandler> mockEventHandler;
        std::shared_ptr<MockInternalFlowMeasuresUpdatedEventHandler> mockEventHandlerInternal;
        std::shared_ptr<ECFMP::EventBus::InternalEventBus> eventBus;
        MockFlowMeasureFilterParser* filterParserRaw;
        MockFlowMeasureMeasureParser* measureParserRaw;
        ECFMP::Api::InternalEventCollection events;
        ECFMP::Api::InternalFlightInformationRegionCollection firs;
        std::unique_ptr<ECFMP::Api::FlowMeasureDataParser> parser;
    };

    TEST_P(FlowMeasureDataParserTest, ItParsesFlowMeasures)
    {
        auto testCase = GetParam();

        // Mock the measure and filter parser returns
        ON_CALL(*filterParserRaw, Parse(testCase.data.at("filters"), testing::_))
                .WillByDefault(testing::Invoke([&](const nlohmann::json& data,
                                                   const ECFMP::Api::InternalEventCollection& events) {
                    return std::make_unique<ECFMP::FlowMeasure::ConcreteFlowMeasureFilters>(
                            std::list<std::shared_ptr<ECFMP::FlowMeasure::AirportFilter>>(),
                            std::list<std::shared_ptr<ECFMP::FlowMeasure::EventFilter>>(),
                            std::list<std::shared_ptr<ECFMP::FlowMeasure::RouteFilter>>(),
                            std::list<std::shared_ptr<ECFMP::FlowMeasure::LevelRangeFilter>>(),
                            std::list<std::shared_ptr<ECFMP::FlowMeasure::MultipleLevelFilter>>(),
                            std::list<std::shared_ptr<ECFMP::FlowMeasure::RangeToDestinationFilter>>(),
                            std::make_shared<Euroscope::MockEuroscopeAircraftFactory>()
                    );
                }));

        ON_CALL(*measureParserRaw, Parse(testCase.data.at("measure")))
                .WillByDefault(testing::Invoke([&](const nlohmann::json& data) {
                    return std::make_unique<ECFMP::FlowMeasure::ConcreteMeasure>(
                            ECFMP::FlowMeasure::MeasureType::GroundStop
                    );
                }));

        auto data = nlohmann::json{{"flow_measures", nlohmann::json::array({testCase.data})}};
        auto flowMeasures = parser->ParseFlowMeasures(data, events, firs);
        EXPECT_EQ(testCase.expectedId, flowMeasures->Get(testCase.expectedId)->Id());
        EXPECT_EQ(testCase.expectedEventId, flowMeasures->Get(testCase.expectedId)->Event()->Id());
        EXPECT_EQ(testCase.expectedIdentifier, flowMeasures->Get(testCase.expectedId)->Identifier());
        EXPECT_EQ(testCase.expectedReason, flowMeasures->Get(testCase.expectedId)->Reason());
        EXPECT_EQ(testCase.expectedStart, flowMeasures->Get(testCase.expectedId)->StartTime());
        EXPECT_EQ(testCase.expectedEnd, flowMeasures->Get(testCase.expectedId)->EndTime());
        EXPECT_EQ(testCase.expectedStatus, flowMeasures->Get(testCase.expectedId)->Status());
        EXPECT_EQ(
                ECFMP::FlowMeasure::MeasureType::GroundStop, flowMeasures->Get(testCase.expectedId)->Measure().Type()
        );

        if (flowMeasures->Get(testCase.expectedId)->Status() == ECFMP::FlowMeasure::MeasureStatus::Withdrawn) {
            EXPECT_EQ(testCase.expectedWithdrawnAt, flowMeasures->Get(testCase.expectedId)->WithdrawnAt());
        }

        auto notifiedFirs = flowMeasures->Get(testCase.expectedId)->NotifiedFlightInformationRegions();
        auto notifiedFirIds = std::list<int>();
        std::transform(
                notifiedFirs.begin(), notifiedFirs.end(), std::back_inserter(notifiedFirIds),
                [](const std::shared_ptr<const ECFMP::FlightInformationRegion::FlightInformationRegion>& fir) {
                    return fir->Id();
                }
        );

        EXPECT_EQ(testCase.expectedNotifiedFirIds, notifiedFirIds);

        // Check that the event was published
        EXPECT_EQ(1, mockEventHandler->GetCallCount());
        EXPECT_EQ(1, mockEventHandlerInternal->GetCallCount());
    }

    INSTANTIATE_TEST_SUITE_P(
            FlowMeasureDataParserTest, FlowMeasureDataParserTest,
            testing::Values(
                    FlowMeasureDataParserTestCase{
                            "withdrawn_measure_that_is_active",
                            nlohmann::json{
                                    {"id", 1},
                                    {"event_id", 1},
                                    {"ident", "EGTT-01A"},
                                    {"reason", "reason 1"},
                                    {"starttime", ECFMP::Date::DateStringFromTimePoint(minusOneHour)},
                                    {"endtime", ECFMP::Date::DateStringFromTimePoint(plusOneHour)},
                                    {"withdrawn_at", ECFMP::Date::DateStringFromTimePoint(now)},
                                    {"measure",
                                     {{"foo", "bar"}}},// Measure is mocked and handled elsewhere, so placeholder
                                    {"filters",
                                     {{"foo", "baz"}}},// Filter is mocked and handled elsewhere, so placeholder
                                    {"notified_flight_information_regions", nlohmann::json::array({1, 2})}},
                            1, 1, "EGTT-01A", "reason 1", minusOneHour, plusOneHour, now,
                            ECFMP::FlowMeasure::MeasureStatus::Withdrawn, std::list<int>({1, 2})},
                    FlowMeasureDataParserTestCase{
                            "withdrawn_measure_that_is_notified",
                            nlohmann::json{
                                    {"id", 1},
                                    {"event_id", 1},
                                    {"ident", "EGTT-01A"},
                                    {"reason", "reason 1"},
                                    {"starttime", ECFMP::Date::DateStringFromTimePoint(plusOneHour)},
                                    {"endtime", ECFMP::Date::DateStringFromTimePoint(plusTwoHours)},
                                    {"withdrawn_at", ECFMP::Date::DateStringFromTimePoint(now)},
                                    {"measure",
                                     {{"foo", "bar"}}},// Measure is mocked and handled elsewhere, so placeholder
                                    {"filters",
                                     {{"foo", "baz"}}},// Filter is mocked and handled elsewhere, so placeholder
                                    {"notified_flight_information_regions", nlohmann::json::array({1, 2})}},
                            1, 1, "EGTT-01A", "reason 1", plusOneHour, plusTwoHours, now,
                            ECFMP::FlowMeasure::MeasureStatus::Withdrawn, std::list<int>({1, 2})},
                    FlowMeasureDataParserTestCase{
                            "withdrawn_measure_that_is_expired",
                            nlohmann::json{
                                    {"id", 1},
                                    {"event_id", 1},
                                    {"ident", "EGTT-01A"},
                                    {"reason", "reason 1"},
                                    {"starttime", ECFMP::Date::DateStringFromTimePoint(minusTwoHours)},
                                    {"endtime", ECFMP::Date::DateStringFromTimePoint(minusOneHour)},
                                    {"withdrawn_at", ECFMP::Date::DateStringFromTimePoint(now)},
                                    {"measure",
                                     {{"foo", "bar"}}},// Measure is mocked and handled elsewhere, so placeholder
                                    {"filters",
                                     {{"foo", "baz"}}},// Filter is mocked and handled elsewhere, so placeholder
                                    {"notified_flight_information_regions", nlohmann::json::array({1, 2})}},
                            1, 1, "EGTT-01A", "reason 1", minusTwoHours, minusOneHour, now,
                            ECFMP::FlowMeasure::MeasureStatus::Withdrawn, std::list<int>({1, 2})},
                    FlowMeasureDataParserTestCase{
                            "active_measure",
                            nlohmann::json{
                                    {"id", 1},
                                    {"event_id", 1},
                                    {"ident", "EGTT-01A"},
                                    {"reason", "reason 1"},
                                    {"starttime", ECFMP::Date::DateStringFromTimePoint(minusOneHour)},
                                    {"endtime", ECFMP::Date::DateStringFromTimePoint(plusOneHour)},
                                    {"withdrawn_at", nlohmann::json::value_t::null},
                                    {"measure",
                                     {{"foo", "bar"}}},// Measure is mocked and handled elsewhere, so placeholder
                                    {"filters",
                                     {{"foo", "baz"}}},// Filter is mocked and handled elsewhere, so placeholder
                                    {"notified_flight_information_regions", nlohmann::json::array({1, 2})}},
                            1, 1, "EGTT-01A", "reason 1", minusOneHour, plusOneHour, now,
                            ECFMP::FlowMeasure::MeasureStatus::Active, std::list<int>({1, 2})},
                    FlowMeasureDataParserTestCase{
                            "notified_measure",
                            nlohmann::json{
                                    {"id", 1},
                                    {"event_id", 1},
                                    {"ident", "EGTT-01A"},
                                    {"reason", "reason 1"},
                                    {"starttime", ECFMP::Date::DateStringFromTimePoint(plusOneHour)},
                                    {"endtime", ECFMP::Date::DateStringFromTimePoint(plusTwoHours)},
                                    {"withdrawn_at", nlohmann::json::value_t::null},
                                    {"measure",
                                     {{"foo", "bar"}}},// Measure is mocked and handled elsewhere, so placeholder
                                    {"filters",
                                     {{"foo", "baz"}}},// Filter is mocked and handled elsewhere, so placeholder
                                    {"notified_flight_information_regions", nlohmann::json::array({1, 2})}},
                            1, 1, "EGTT-01A", "reason 1", plusOneHour, plusTwoHours, now,
                            ECFMP::FlowMeasure::MeasureStatus::Notified, std::list<int>({1, 2})},
                    FlowMeasureDataParserTestCase{
                            "expired_measure",
                            nlohmann::json{
                                    {"id", 1},
                                    {"event_id", 1},
                                    {"ident", "EGTT-01A"},
                                    {"reason", "reason 1"},
                                    {"starttime", ECFMP::Date::DateStringFromTimePoint(minusTwoHours)},
                                    {"endtime", ECFMP::Date::DateStringFromTimePoint(minusOneHour)},
                                    {"withdrawn_at", nlohmann::json::value_t::null},
                                    {"measure",
                                     {{"foo", "bar"}}},// Measure is mocked and handled elsewhere, so placeholder
                                    {"filters",
                                     {{"foo", "baz"}}},// Filter is mocked and handled elsewhere, so placeholder
                                    {"notified_flight_information_regions", nlohmann::json::array({1, 2})}},
                            1, 1, "EGTT-01A", "reason 1", minusTwoHours, minusOneHour, now,
                            ECFMP::FlowMeasure::MeasureStatus::Expired, std::list<int>({1, 2})}
            ),
            [](const testing::TestParamInfo<FlowMeasureDataParserTestCase>& info) {
                return info.param.description;
            }
    );

    class FlowMeasureDataParserMultipleMeasuresTest : public testing::Test
    {
        public:
        void SetUp() override
        {
            // Create the mock parsers
            auto filterParser = std::make_unique<MockFlowMeasureFilterParser>();
            filterParserRaw = filterParser.get();
            auto measureParser = std::make_unique<MockFlowMeasureMeasureParser>();
            measureParserRaw = measureParser.get();

            eventBus = ECFMP::EventBus::MakeEventBus();
            mockEventHandler = std::make_shared<MockFlowMeasuresUpdatedEventHandler>(2);
            mockEventHandlerInternal = std::make_shared<MockInternalFlowMeasuresUpdatedEventHandler>(2);
            eventBus->SubscribeSync<ECFMP::Plugin::FlowMeasuresUpdatedEvent>(mockEventHandler);
            eventBus->SubscribeSync<ECFMP::Plugin::InternalFlowMeasuresUpdatedEvent>(mockEventHandlerInternal);

            customFilters =
                    std::make_shared<std::vector<std::shared_ptr<ECFMP::FlowMeasure::CustomFlowMeasureFilter>>>();
            parser = std::make_unique<ECFMP::Api::FlowMeasureDataParser>(
                    std::move(filterParser), std::move(measureParser), std::make_shared<Log::MockLogger>(), eventBus,
                    customFilters
            );

            firs.Add(std::make_shared<ECFMP::FlightInformationRegion::ConcreteFlightInformationRegion>(
                    1, "EGTT", "London"
            ));
            firs.Add(std::make_shared<ECFMP::FlightInformationRegion::ConcreteFlightInformationRegion>(
                    2, "EGPX", "Scottish"
            ));

            events.Add(std::make_shared<ECFMP::Event::ConcreteEvent>(
                    1, "Test event", now, plusOneHour, firs.Get(1), "abc",
                    std::vector<std::shared_ptr<ECFMP::Event::EventParticipant>>{}
            ));
        }

        std::shared_ptr<std::vector<std::shared_ptr<ECFMP::FlowMeasure::CustomFlowMeasureFilter>>> customFilters;
        std::shared_ptr<MockFlowMeasuresUpdatedEventHandler> mockEventHandler;
        std::shared_ptr<MockInternalFlowMeasuresUpdatedEventHandler> mockEventHandlerInternal;
        std::shared_ptr<ECFMP::EventBus::InternalEventBus> eventBus;
        MockFlowMeasureFilterParser* filterParserRaw;
        MockFlowMeasureMeasureParser* measureParserRaw;
        ECFMP::Api::InternalEventCollection events;
        ECFMP::Api::InternalFlightInformationRegionCollection firs;
        std::unique_ptr<ECFMP::Api::FlowMeasureDataParser> parser;
    };

    TEST_F(FlowMeasureDataParserMultipleMeasuresTest, ItParsesFlowMeasures)
    {
        // Mock the measure and filter parser returns
        ON_CALL(*filterParserRaw, Parse(testing::_, testing::_))
                .WillByDefault(testing::Invoke([&](const nlohmann::json& data,
                                                   const ECFMP::Api::InternalEventCollection& events) {
                    return std::make_unique<ECFMP::FlowMeasure::ConcreteFlowMeasureFilters>(
                            std::list<std::shared_ptr<ECFMP::FlowMeasure::AirportFilter>>(),
                            std::list<std::shared_ptr<ECFMP::FlowMeasure::EventFilter>>(),
                            std::list<std::shared_ptr<ECFMP::FlowMeasure::RouteFilter>>(),
                            std::list<std::shared_ptr<ECFMP::FlowMeasure::LevelRangeFilter>>(),
                            std::list<std::shared_ptr<ECFMP::FlowMeasure::MultipleLevelFilter>>(),
                            std::list<std::shared_ptr<ECFMP::FlowMeasure::RangeToDestinationFilter>>(),
                            std::make_shared<Euroscope::MockEuroscopeAircraftFactory>()
                    );
                }));

        ON_CALL(*measureParserRaw, Parse(testing::_)).WillByDefault(testing::Invoke([&](const nlohmann::json& data) {
            return std::make_unique<ECFMP::FlowMeasure::ConcreteMeasure>(ECFMP::FlowMeasure::MeasureType::GroundStop);
        }));

        const auto measure1 = nlohmann::json{
                {"id", 1},
                {"event_id", 1},
                {"ident", "EGTT-01A"},
                {"reason", "reason 1"},
                {"starttime", ECFMP::Date::DateStringFromTimePoint(plusOneHour)},
                {"endtime", ECFMP::Date::DateStringFromTimePoint(plusTwoHours)},
                {"withdrawn_at", nlohmann::json::value_t::null},
                {"measure", {{"foo", "bar"}}},// Measure is mocked and handled elsewhere, so placeholder
                {"filters", {{"foo", "baz"}}},// Filter is mocked and handled elsewhere, so placeholder
                {"notified_flight_information_regions", nlohmann::json::array({1, 2})}};

        const auto measure2 = nlohmann::json{
                {"id", 2},
                {"event_id", 1},
                {"ident", "EGTT-01B"},
                {"reason", "reason 1"},
                {"starttime", ECFMP::Date::DateStringFromTimePoint(plusOneHour)},
                {"endtime", ECFMP::Date::DateStringFromTimePoint(plusTwoHours)},
                {"withdrawn_at", nlohmann::json::value_t::null},
                {"measure", {{"foo", "bar"}}},// Measure is mocked and handled elsewhere, so placeholder
                {"filters", {{"foo", "baz"}}},// Filter is mocked and handled elsewhere, so placeholder
                {"notified_flight_information_regions", nlohmann::json::array({1, 2})}};

        auto data = nlohmann::json{{"flow_measures", nlohmann::json::array({measure1, measure2})}};
        auto flowMeasures = parser->ParseFlowMeasures(data, events, firs);
        EXPECT_EQ(2, flowMeasures->Count());
        EXPECT_EQ("EGTT-01A", flowMeasures->begin()->Identifier());
        EXPECT_EQ("EGTT-01B", (++flowMeasures->begin())->Identifier());
    }

    struct FlowMeasureDataParserBadDataTestCase {
        std::string description;
        nlohmann::json data;
    };

    class FlowMeasureDataParserBadDataTest : public testing::TestWithParam<FlowMeasureDataParserBadDataTestCase>
    {
        public:
        void SetUp() override
        {
            // Create the mock parsers
            auto filterParser = std::make_unique<MockFlowMeasureFilterParser>();
            filterParserRaw = filterParser.get();
            auto measureParser = std::make_unique<MockFlowMeasureMeasureParser>();
            measureParserRaw = measureParser.get();

            eventBus = ECFMP::EventBus::MakeEventBus();
            mockEventHandler = std::make_shared<MockFlowMeasuresUpdatedEventHandler>(0);
            mockEventHandlerInternal = std::make_shared<MockInternalFlowMeasuresUpdatedEventHandler>(0);
            eventBus->SubscribeSync<ECFMP::Plugin::FlowMeasuresUpdatedEvent>(mockEventHandler);
            eventBus->SubscribeSync<ECFMP::Plugin::InternalFlowMeasuresUpdatedEvent>(mockEventHandlerInternal);

            customFilters =
                    std::make_shared<std::vector<std::shared_ptr<ECFMP::FlowMeasure::CustomFlowMeasureFilter>>>();
            parser = std::make_unique<ECFMP::Api::FlowMeasureDataParser>(
                    std::move(filterParser), std::move(measureParser), std::make_shared<Log::MockLogger>(), eventBus,
                    customFilters
            );

            firs.Add(std::make_shared<ECFMP::FlightInformationRegion::ConcreteFlightInformationRegion>(
                    1, "EGTT", "London"
            ));
            firs.Add(std::make_shared<ECFMP::FlightInformationRegion::ConcreteFlightInformationRegion>(
                    2, "EGPX", "Scottish"
            ));

            events.Add(std::make_shared<ECFMP::Event::ConcreteEvent>(
                    1, "Test event", now, plusOneHour, firs.Get(1), "abc",
                    std::vector<std::shared_ptr<ECFMP::Event::EventParticipant>>{}
            ));
        }

        std::shared_ptr<std::vector<std::shared_ptr<ECFMP::FlowMeasure::CustomFlowMeasureFilter>>> customFilters;
        std::shared_ptr<MockFlowMeasuresUpdatedEventHandler> mockEventHandler;
        std::shared_ptr<MockInternalFlowMeasuresUpdatedEventHandler> mockEventHandlerInternal;
        std::shared_ptr<ECFMP::EventBus::InternalEventBus> eventBus;
        MockFlowMeasureFilterParser* filterParserRaw;
        MockFlowMeasureMeasureParser* measureParserRaw;
        ECFMP::Api::InternalEventCollection events;
        ECFMP::Api::InternalFlightInformationRegionCollection firs;
        std::unique_ptr<ECFMP::Api::FlowMeasureDataParser> parser;
    };

    TEST_P(FlowMeasureDataParserBadDataTest, ItHandlesBadData)
    {
        auto testCase = GetParam();

        // Mock the measure and filter parser returns
        ON_CALL(*filterParserRaw, Parse(testing::_, testing::_))
                .WillByDefault(testing::Invoke([&](const nlohmann::json& data,
                                                   const ECFMP::Api::InternalEventCollection& events) {
                    return std::make_unique<ECFMP::FlowMeasure::ConcreteFlowMeasureFilters>(
                            std::list<std::shared_ptr<ECFMP::FlowMeasure::AirportFilter>>(),
                            std::list<std::shared_ptr<ECFMP::FlowMeasure::EventFilter>>(),
                            std::list<std::shared_ptr<ECFMP::FlowMeasure::RouteFilter>>(),
                            std::list<std::shared_ptr<ECFMP::FlowMeasure::LevelRangeFilter>>(),
                            std::list<std::shared_ptr<ECFMP::FlowMeasure::MultipleLevelFilter>>(),
                            std::list<std::shared_ptr<ECFMP::FlowMeasure::RangeToDestinationFilter>>(),
                            std::make_shared<Euroscope::MockEuroscopeAircraftFactory>()
                    );
                }));

        ON_CALL(*measureParserRaw, Parse(testing::_)).WillByDefault(testing::Invoke([&](const nlohmann::json& data) {
            return std::make_unique<ECFMP::FlowMeasure::ConcreteMeasure>(ECFMP::FlowMeasure::MeasureType::GroundStop);
        }));

        // If the data is an object, wrap it in a flow_measures array - otherwise we're testing bad data type.
        const auto dataUnderTest = testCase.data.is_object()
                ? nlohmann::json{{"flow_measures", nlohmann::json::array({testCase.data})}}
                : testCase.data;

        auto flowMeasures = parser->ParseFlowMeasures(dataUnderTest, events, firs);
        if (!testCase.data.is_object()) {
            EXPECT_EQ(nullptr, flowMeasures);
        }
        else {
            EXPECT_EQ(0, flowMeasures->Count());
        }

        // Check that the event handler was called
        EXPECT_EQ(1, mockEventHandler->GetCallCount());
        EXPECT_EQ(1, mockEventHandlerInternal->GetCallCount());
    }

    INSTANTIATE_TEST_SUITE_P(
            FlowMeasureDataParserBadDataTest, FlowMeasureDataParserBadDataTest,
            testing::Values(
                    FlowMeasureDataParserBadDataTestCase{
                            "missing_id",
                            nlohmann::json{
                                    {"event_id", 1},
                                    {"ident", "EGTT-01A"},
                                    {"reason", "reason 1"},
                                    {"starttime", ECFMP::Date::DateStringFromTimePoint(minusOneHour)},
                                    {"endtime", ECFMP::Date::DateStringFromTimePoint(plusOneHour)},
                                    {"withdrawn_at", ECFMP::Date::DateStringFromTimePoint(now)},
                                    {"measure",
                                     {{"foo", "bar"}}},// Measure is mocked and handled elsewhere, so placeholder
                                    {"filters",
                                     {{"foo", "baz"}}},// Filter is mocked and handled elsewhere, so placeholder
                                    {"notified_flight_information_regions", nlohmann::json::array({1, 2})}}},
                    FlowMeasureDataParserBadDataTestCase{
                            "id_not_integer",
                            nlohmann::json{
                                    {"id", "foo"},
                                    {"event_id", 1},
                                    {"ident", "EGTT-01A"},
                                    {"reason", "reason 1"},
                                    {"starttime", ECFMP::Date::DateStringFromTimePoint(minusOneHour)},
                                    {"endtime", ECFMP::Date::DateStringFromTimePoint(plusOneHour)},
                                    {"withdrawn_at", ECFMP::Date::DateStringFromTimePoint(now)},
                                    {"measure",
                                     {{"foo", "bar"}}},// Measure is mocked and handled elsewhere, so placeholder
                                    {"filters",
                                     {{"foo", "baz"}}},// Filter is mocked and handled elsewhere, so placeholder
                                    {"notified_flight_information_regions", nlohmann::json::array({1, 2})}}},
                    FlowMeasureDataParserBadDataTestCase{
                            "missing_ident",
                            nlohmann::json{
                                    {"id", 1},
                                    {"event_id", 1},
                                    {"reason", "reason 1"},
                                    {"starttime", ECFMP::Date::DateStringFromTimePoint(minusOneHour)},
                                    {"endtime", ECFMP::Date::DateStringFromTimePoint(plusOneHour)},
                                    {"withdrawn_at", ECFMP::Date::DateStringFromTimePoint(now)},
                                    {"measure",
                                     {{"foo", "bar"}}},// Measure is mocked and handled elsewhere, so placeholder
                                    {"filters",
                                     {{"foo", "baz"}}},// Filter is mocked and handled elsewhere, so placeholder
                                    {"notified_flight_information_regions", nlohmann::json::array({1, 2})}}},
                    FlowMeasureDataParserBadDataTestCase{
                            "ident_not_string",
                            nlohmann::json{
                                    {"id", 1},
                                    {"event_id", 1},
                                    {"ident", 123},
                                    {"reason", "reason 1"},
                                    {"starttime", ECFMP::Date::DateStringFromTimePoint(minusOneHour)},
                                    {"endtime", ECFMP::Date::DateStringFromTimePoint(plusOneHour)},
                                    {"withdrawn_at", ECFMP::Date::DateStringFromTimePoint(now)},
                                    {"measure",
                                     {{"foo", "bar"}}},// Measure is mocked and handled elsewhere, so placeholder
                                    {"filters",
                                     {{"foo", "baz"}}},// Filter is mocked and handled elsewhere, so placeholder
                                    {"notified_flight_information_regions", nlohmann::json::array({1, 2})}}},

                    FlowMeasureDataParserBadDataTestCase{
                            "missing_event_id",
                            nlohmann::json{
                                    {"id", 1},
                                    {"ident", "EGTT-01A"},
                                    {"reason", "reason 1"},
                                    {"starttime", ECFMP::Date::DateStringFromTimePoint(minusOneHour)},
                                    {"endtime", ECFMP::Date::DateStringFromTimePoint(plusOneHour)},
                                    {"withdrawn_at", ECFMP::Date::DateStringFromTimePoint(now)},
                                    {"measure",
                                     {{"foo", "bar"}}},// Measure is mocked and handled elsewhere, so placeholder
                                    {"filters",
                                     {{"foo", "baz"}}},// Filter is mocked and handled elsewhere, so placeholder
                                    {"notified_flight_information_regions", nlohmann::json::array({1, 2})}}},
                    FlowMeasureDataParserBadDataTestCase{
                            "event_id_not_integer",
                            nlohmann::json{
                                    {"id", 1},
                                    {"event_id", "foo"},
                                    {"ident", "EGTT-01A"},
                                    {"reason", "reason 1"},
                                    {"starttime", ECFMP::Date::DateStringFromTimePoint(minusOneHour)},
                                    {"endtime", ECFMP::Date::DateStringFromTimePoint(plusOneHour)},
                                    {"withdrawn_at", ECFMP::Date::DateStringFromTimePoint(now)},
                                    {"measure",
                                     {{"foo", "bar"}}},// Measure is mocked and handled elsewhere, so placeholder
                                    {"filters",
                                     {{"foo", "baz"}}},// Filter is mocked and handled elsewhere, so placeholder
                                    {"notified_flight_information_regions", nlohmann::json::array({1, 2})}}},

                    FlowMeasureDataParserBadDataTestCase{
                            "event_for_id_not_found",
                            nlohmann::json{
                                    {"id", 1},
                                    {"event_id", 999},
                                    {"ident", "EGTT-01A"},
                                    {"reason", "reason 1"},
                                    {"starttime", ECFMP::Date::DateStringFromTimePoint(minusOneHour)},
                                    {"endtime", ECFMP::Date::DateStringFromTimePoint(plusOneHour)},
                                    {"withdrawn_at", ECFMP::Date::DateStringFromTimePoint(now)},
                                    {"measure",
                                     {{"foo", "bar"}}},// Measure is mocked and handled elsewhere, so placeholder
                                    {"filters",
                                     {{"foo", "baz"}}},// Filter is mocked and handled elsewhere, so placeholder
                                    {"notified_flight_information_regions", nlohmann::json::array({1, 2})}}},
                    FlowMeasureDataParserBadDataTestCase{

                            "missing_reason",
                            nlohmann::json{
                                    {"id", 1},
                                    {"event_id", 1},
                                    {"ident", "EGTT-01A"},
                                    {"starttime", ECFMP::Date::DateStringFromTimePoint(minusOneHour)},
                                    {"endtime", ECFMP::Date::DateStringFromTimePoint(plusOneHour)},
                                    {"withdrawn_at", ECFMP::Date::DateStringFromTimePoint(now)},
                                    {"measure",
                                     {{"foo", "bar"}}},// Measure is mocked and handled elsewhere, so placeholder
                                    {"filters",
                                     {{"foo", "baz"}}},// Filter is mocked and handled elsewhere, so placeholder
                                    {"notified_flight_information_regions", nlohmann::json::array({1, 2})}}},
                    FlowMeasureDataParserBadDataTestCase{
                            "reason_not_string",
                            nlohmann::json{
                                    {"id", 1},
                                    {"event_id", 1},
                                    {"ident", "EGTT-01A"},
                                    {"reason", 123},
                                    {"starttime", ECFMP::Date::DateStringFromTimePoint(minusOneHour)},
                                    {"endtime", ECFMP::Date::DateStringFromTimePoint(plusOneHour)},
                                    {"withdrawn_at", ECFMP::Date::DateStringFromTimePoint(now)},
                                    {"measure",
                                     {{"foo", "bar"}}},// Measure is mocked and handled elsewhere, so placeholder
                                    {"filters",
                                     {{"foo", "baz"}}},// Filter is mocked and handled elsewhere, so placeholder
                                    {"notified_flight_information_regions", nlohmann::json::array({1, 2})}}},

                    FlowMeasureDataParserBadDataTestCase{

                            "missing_starttime",
                            nlohmann::json{
                                    {"id", 1},
                                    {"event_id", 1},
                                    {"ident", "EGTT-01A"},
                                    {"reason", "reason 1"},
                                    {"endtime", ECFMP::Date::DateStringFromTimePoint(plusOneHour)},
                                    {"withdrawn_at", ECFMP::Date::DateStringFromTimePoint(now)},
                                    {"measure",
                                     {{"foo", "bar"}}},// Measure is mocked and handled elsewhere, so placeholder
                                    {"filters",
                                     {{"foo", "baz"}}},// Filter is mocked and handled elsewhere, so placeholder
                                    {"notified_flight_information_regions", nlohmann::json::array({1, 2})}}},

                    FlowMeasureDataParserBadDataTestCase{
                            "starttime_not_string",
                            nlohmann::json{
                                    {"id", 1},
                                    {"event_id", 1},
                                    {"ident", "EGTT-01A"},
                                    {"reason", "reason 1"},
                                    {"starttime", 123},
                                    {"endtime", ECFMP::Date::DateStringFromTimePoint(plusOneHour)},
                                    {"withdrawn_at", ECFMP::Date::DateStringFromTimePoint(now)},
                                    {"measure",
                                     {{"foo", "bar"}}},// Measure is mocked and handled elsewhere, so placeholder
                                    {"filters",
                                     {{"foo", "baz"}}},// Filter is mocked and handled elsewhere, so placeholder
                                    {"notified_flight_information_regions", nlohmann::json::array({1, 2})}}},

                    FlowMeasureDataParserBadDataTestCase{
                            "invalid_starttime",
                            nlohmann::json{
                                    {"id", 1},
                                    {"event_id", 1},
                                    {"ident", "EGTT-01A"},
                                    {"reason", "reason 1"},
                                    {"starttime", "foo"},
                                    {"endtime", ECFMP::Date::DateStringFromTimePoint(plusOneHour)},
                                    {"withdrawn_at", ECFMP::Date::DateStringFromTimePoint(now)},
                                    {"measure",
                                     {{"foo", "bar"}}},// Measure is mocked and handled elsewhere, so placeholder
                                    {"filters",
                                     {{"foo", "baz"}}},// Filter is mocked and handled elsewhere, so placeholder
                                    {"notified_flight_information_regions", nlohmann::json::array({1, 2})}}},

                    FlowMeasureDataParserBadDataTestCase{
                            "missing_endtime",
                            nlohmann::json{
                                    {"id", 1},
                                    {"event_id", 1},
                                    {"ident", "EGTT-01A"},
                                    {"reason", "reason 1"},
                                    {"starttime", ECFMP::Date::DateStringFromTimePoint(minusOneHour)},
                                    {"withdrawn_at", ECFMP::Date::DateStringFromTimePoint(now)},
                                    {"measure",
                                     {{"foo", "bar"}}},// Measure is mocked and handled elsewhere, so placeholder
                                    {"filters",
                                     {{"foo", "baz"}}},// Filter is mocked and handled elsewhere, so placeholder
                                    {"notified_flight_information_regions", nlohmann::json::array({1, 2})}}},

                    FlowMeasureDataParserBadDataTestCase{
                            "endtime_not_string",
                            nlohmann::json{
                                    {"id", 1},
                                    {"event_id", 1},
                                    {"ident", "EGTT-01A"},
                                    {"reason", "reason 1"},
                                    {"starttime", ECFMP::Date::DateStringFromTimePoint(minusOneHour)},
                                    {"endtime", 123},
                                    {"withdrawn_at", ECFMP::Date::DateStringFromTimePoint(now)},
                                    {"measure",
                                     {{"foo", "bar"}}},// Measure is mocked and handled elsewhere, so placeholder
                                    {"filters",
                                     {{"foo", "baz"}}},// Filter is mocked and handled elsewhere, so placeholder
                                    {"notified_flight_information_regions", nlohmann::json::array({1, 2})}}},

                    FlowMeasureDataParserBadDataTestCase{

                            "invalid_endtime",
                            nlohmann::json{
                                    {"id", 1},
                                    {"event_id", 1},
                                    {"ident", "EGTT-01A"},
                                    {"reason", "reason 1"},
                                    {"starttime", ECFMP::Date::DateStringFromTimePoint(minusOneHour)},
                                    {"endtime", "foo"},
                                    {"withdrawn_at", ECFMP::Date::DateStringFromTimePoint(now)},
                                    {"measure",
                                     {{"foo", "bar"}}},// Measure is mocked and handled elsewhere, so placeholder
                                    {"filters",
                                     {{"foo", "baz"}}},// Filter is mocked and handled elsewhere, so placeholder
                                    {"notified_flight_information_regions", nlohmann::json::array({1, 2})}}},

                    FlowMeasureDataParserBadDataTestCase{
                            "missing_withdrawn_at",
                            nlohmann::json{
                                    {"id", 1},
                                    {"event_id", 1},
                                    {"ident", "EGTT-01A"},
                                    {"reason", "reason 1"},
                                    {"starttime", ECFMP::Date::DateStringFromTimePoint(minusOneHour)},
                                    {"endtime", ECFMP::Date::DateStringFromTimePoint(plusOneHour)},
                                    {"measure",
                                     {{"foo", "bar"}}},// Measure is mocked and handled elsewhere, so placeholder
                                    {"filters",
                                     {{"foo", "baz"}}},// Filter is mocked and handled elsewhere, so placeholder
                                    {"notified_flight_information_regions", nlohmann::json::array({1, 2})}}},

                    FlowMeasureDataParserBadDataTestCase{
                            "withdrawn_at_not_string",
                            nlohmann::json{
                                    {"id", 1},
                                    {"event_id", 1},
                                    {"ident", "EGTT-01A"},
                                    {"reason", "reason 1"},
                                    {"starttime", ECFMP::Date::DateStringFromTimePoint(minusOneHour)},
                                    {"endtime", ECFMP::Date::DateStringFromTimePoint(plusOneHour)},
                                    {"withdrawn_at", 123},
                                    {"measure",
                                     {{"foo", "bar"}}},// Measure is mocked and handled elsewhere, so placeholder
                                    {"filters",
                                     {{"foo", "baz"}}},// Filter is mocked and handled elsewhere, so placeholder
                                    {"notified_flight_information_regions", nlohmann::json::array({1, 2})}}},

                    FlowMeasureDataParserBadDataTestCase{
                            "invalid_withdrawn_at",
                            nlohmann::json{
                                    {"id", 1},
                                    {"event_id", 1},
                                    {"ident", "EGTT-01A"},
                                    {"reason", "reason 1"},
                                    {"starttime", ECFMP::Date::DateStringFromTimePoint(minusOneHour)},
                                    {"endtime", ECFMP::Date::DateStringFromTimePoint(plusOneHour)},
                                    {"withdrawn_at", "foo"},
                                    {"measure",
                                     {{"foo", "bar"}}},// Measure is mocked and handled elsewhere, so placeholder
                                    {"filters",
                                     {{"foo", "baz"}}},// Filter is mocked and handled elsewhere, so placeholder
                                    {"notified_flight_information_regions", nlohmann::json::array({1, 2})}}},

                    FlowMeasureDataParserBadDataTestCase{
                            "missing_measure",
                            nlohmann::json{
                                    {"id", 1},
                                    {"event_id", 1},
                                    {"ident", "EGTT-01A"},
                                    {"reason", "reason 1"},
                                    {"starttime", ECFMP::Date::DateStringFromTimePoint(minusOneHour)},
                                    {"endtime", ECFMP::Date::DateStringFromTimePoint(plusOneHour)},
                                    {"withdrawn_at", ECFMP::Date::DateStringFromTimePoint(now)},
                                    {"filters",
                                     {{"foo", "baz"}}},// Filter is mocked and handled elsewhere, so placeholder
                                    {"notified_flight_information_regions", nlohmann::json::array({1, 2})}}},
                    FlowMeasureDataParserBadDataTestCase{
                            "missing_filters",
                            nlohmann::json{
                                    {"id", 1},
                                    {"event_id", 1},
                                    {"ident", "EGTT-01A"},
                                    {"reason", "reason 1"},
                                    {"starttime", ECFMP::Date::DateStringFromTimePoint(minusOneHour)},
                                    {"endtime", ECFMP::Date::DateStringFromTimePoint(plusOneHour)},
                                    {"withdrawn_at", ECFMP::Date::DateStringFromTimePoint(now)},
                                    {"measure",
                                     {{"foo", "bar"}}},// Measure is mocked and handled elsewhere, so placeholder
                                    {"notified_flight_information_regions", nlohmann::json::array({1, 2})}}},

                    FlowMeasureDataParserBadDataTestCase{
                            "missing_notified_flight_information_regions",
                            nlohmann::json{
                                    {"id", 1},
                                    {"event_id", 1},
                                    {"ident", "EGTT-01A"},
                                    {"reason", "reason 1"},
                                    {"starttime", ECFMP::Date::DateStringFromTimePoint(minusOneHour)},
                                    {"endtime", ECFMP::Date::DateStringFromTimePoint(plusOneHour)},
                                    {"withdrawn_at", ECFMP::Date::DateStringFromTimePoint(now)},
                                    {"measure",
                                     {{"foo", "bar"}}},// Measure is mocked and handled elsewhere, so placeholder
                                    {"filters",
                                     {{"foo", "baz"}}}}},// Filter is mocked and handled elsewhere, so placeholder

                    FlowMeasureDataParserBadDataTestCase{
                            "notified_flight_information_regions_not_array",
                            nlohmann::json{
                                    {"id", 1},
                                    {"event_id", 1},
                                    {"ident", "EGTT-01A"},
                                    {"reason", "reason 1"},
                                    {"starttime", ECFMP::Date::DateStringFromTimePoint(minusOneHour)},
                                    {"endtime", ECFMP::Date::DateStringFromTimePoint(plusOneHour)},
                                    {"withdrawn_at", ECFMP::Date::DateStringFromTimePoint(now)},
                                    {"measure",
                                     {{"foo", "bar"}}},// Measure is mocked and handled elsewhere, so placeholder
                                    {"filters",
                                     {{"foo", "baz"}}},// Filter is mocked and handled elsewhere, so placeholder
                                    {"notified_flight_information_regions", 123}}},

                    FlowMeasureDataParserBadDataTestCase{
                            "notified_flight_information_regions_not_int",
                            nlohmann::json{
                                    {"id", 1},
                                    {"event_id", 1},
                                    {"ident", "EGTT-01A"},
                                    {"reason", "reason 1"},
                                    {"starttime", ECFMP::Date::DateStringFromTimePoint(minusOneHour)},
                                    {"endtime", ECFMP::Date::DateStringFromTimePoint(plusOneHour)},
                                    {"withdrawn_at", ECFMP::Date::DateStringFromTimePoint(now)},
                                    {"measure",
                                     {{"foo", "bar"}}},// Measure is mocked and handled elsewhere, so placeholder
                                    {"filters",
                                     {{"foo", "baz"}}},// Filter is mocked and handled elsewhere, so placeholder
                                    {"notified_flight_information_regions", nlohmann::json::array({"foo", "bar"})}}},

                    FlowMeasureDataParserBadDataTestCase{
                            "notified_flight_information_regions_not_found",
                            nlohmann::json{
                                    {"id", 1},
                                    {"event_id", 1},
                                    {"reason", "reason 1"},
                                    {"starttime", ECFMP::Date::DateStringFromTimePoint(minusOneHour)},
                                    {"endtime", ECFMP::Date::DateStringFromTimePoint(plusOneHour)},
                                    {"withdrawn_at", ECFMP::Date::DateStringFromTimePoint(now)},
                                    {"measure",
                                     {{"foo", "bar"}}},// Measure is mocked and handled elsewhere, so placeholder
                                    {"filters",
                                     {{"foo", "baz"}}},// Filter is mocked and handled elsewhere, so placeholder
                                    {"notified_flight_information_regions", nlohmann::json::array({1, 999})}}}

            ),
            [](const testing::TestParamInfo<FlowMeasureDataParserBadDataTestCase>& info) {
                return info.param.description;
            }
    );

    class FlowMeasureDataParserBizarreDataTest : public testing::Test
    {
        public:
        void SetUp() override
        {
            // Create the mock parsers
            auto filterParser = std::make_unique<MockFlowMeasureFilterParser>();
            filterParserRaw = filterParser.get();
            auto measureParser = std::make_unique<MockFlowMeasureMeasureParser>();
            measureParserRaw = measureParser.get();

            eventBus = ECFMP::EventBus::MakeEventBus();
            mockEventHandler = std::make_shared<MockFlowMeasuresUpdatedEventHandler>(0);
            mockEventHandlerInternal = std::make_shared<MockInternalFlowMeasuresUpdatedEventHandler>(0);
            eventBus->SubscribeSync<ECFMP::Plugin::FlowMeasuresUpdatedEvent>(mockEventHandler);
            eventBus->SubscribeSync<ECFMP::Plugin::InternalFlowMeasuresUpdatedEvent>(mockEventHandlerInternal);

            customFilters =
                    std::make_shared<std::vector<std::shared_ptr<ECFMP::FlowMeasure::CustomFlowMeasureFilter>>>();
            parser = std::make_unique<ECFMP::Api::FlowMeasureDataParser>(
                    std::move(filterParser), std::move(measureParser), std::make_shared<Log::MockLogger>(), eventBus,
                    customFilters
            );

            firs.Add(std::make_shared<ECFMP::FlightInformationRegion::ConcreteFlightInformationRegion>(
                    1, "EGTT", "London"
            ));
            firs.Add(std::make_shared<ECFMP::FlightInformationRegion::ConcreteFlightInformationRegion>(
                    2, "EGPX", "Scottish"
            ));

            events.Add(std::make_shared<ECFMP::Event::ConcreteEvent>(
                    1, "Test event", now, plusOneHour, firs.Get(1), "abc",
                    std::vector<std::shared_ptr<ECFMP::Event::EventParticipant>>{}
            ));
        }

        std::shared_ptr<std::vector<std::shared_ptr<ECFMP::FlowMeasure::CustomFlowMeasureFilter>>> customFilters;
        std::shared_ptr<MockFlowMeasuresUpdatedEventHandler> mockEventHandler;
        std::shared_ptr<MockInternalFlowMeasuresUpdatedEventHandler> mockEventHandlerInternal;
        std::shared_ptr<ECFMP::EventBus::InternalEventBus> eventBus;
        MockFlowMeasureFilterParser* filterParserRaw;
        MockFlowMeasureMeasureParser* measureParserRaw;
        ECFMP::Api::InternalEventCollection events;
        ECFMP::Api::InternalFlightInformationRegionCollection firs;
        std::unique_ptr<ECFMP::Api::FlowMeasureDataParser> parser;
    };

    TEST_F(FlowMeasureDataParserBizarreDataTest, ItReturnsNullptrIfJsonNotObject)
    {
        auto result = parser->ParseFlowMeasures(nlohmann::json::array(), events, firs);
        EXPECT_EQ(nullptr, result);
        EXPECT_EQ(0, mockEventHandler->GetCallCount());
        EXPECT_EQ(0, mockEventHandlerInternal->GetCallCount());
    }

    TEST_F(FlowMeasureDataParserBizarreDataTest, ItReturnsNullptrIfJsonDoesntContainFlowMeasuresKey)
    {
        auto result = parser->ParseFlowMeasures(nlohmann::json{{"foo", "bar"}}, events, firs);
        EXPECT_EQ(nullptr, result);
        EXPECT_EQ(0, mockEventHandler->GetCallCount());
        EXPECT_EQ(0, mockEventHandlerInternal->GetCallCount());
    }

    TEST_F(FlowMeasureDataParserBizarreDataTest, ItReturnsNullptrIfFlowMeasuresKeyIsNotArray)
    {
        auto result = parser->ParseFlowMeasures(nlohmann::json{{"flow_measures", "bar"}}, events, firs);
        EXPECT_EQ(nullptr, result);
        EXPECT_EQ(0, mockEventHandler->GetCallCount());
        EXPECT_EQ(0, mockEventHandlerInternal->GetCallCount());
    }
}// namespace ECFMPTest::Api
