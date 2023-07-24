#include "api/FlightInformationRegionDataParser.h"
#include "ECFMP/eventbus/EventListener.h"
#include "api/InternalElementCollectionTypes.h"
#include "eventbus/InternalEventBusFactory.h"
#include "mock/MockLogger.h"
#include "nlohmann/json.hpp"
#include "plugin/InternalSdkEvents.h"
#include <memory>

namespace ECFMPTest::Api {

    class MockFlightInformationRegionsUpdatedEventHandler
        : public ECFMP::EventBus::EventListener<ECFMP::Plugin::FlightInformationRegionsUpdatedEvent>
    {
        public:
        explicit MockFlightInformationRegionsUpdatedEventHandler(int expectedItemCount)
            : expectedItemCount(expectedItemCount)
        {}

        void OnEvent(const ECFMP::Plugin::FlightInformationRegionsUpdatedEvent& event) override
        {
            callCount++;
            EXPECT_EQ(expectedItemCount, event.firs->Count());
        }

        [[nodiscard]] auto GetCallCount() const -> int
        {
            return callCount;
        }

        private:
        int expectedItemCount;

        int callCount = 0;
    };

    class FlightInformationRegionDataParserTest : public testing::Test
    {
        public:
        FlightInformationRegionDataParserTest()
            : mockEventHandler(std::make_shared<MockFlightInformationRegionsUpdatedEventHandler>(2)),
              eventBus(ECFMP::EventBus::MakeEventBus()),
              mockLogger(std::make_shared<testing::NiceMock<Log::MockLogger>>()), parser(mockLogger, eventBus)
        {
            // Add mock listener to event bus
            eventBus->SubscribeSync<ECFMP::Plugin::FlightInformationRegionsUpdatedEvent>(mockEventHandler);
        }

        std::shared_ptr<MockFlightInformationRegionsUpdatedEventHandler> mockEventHandler;
        std::shared_ptr<ECFMP::EventBus::InternalEventBus> eventBus;
        std::shared_ptr<testing::NiceMock<Log::MockLogger>> mockLogger;
        ECFMP::Api::FlightInformationRegionDataParser parser;
    };

    TEST_F(FlightInformationRegionDataParserTest, ItReturnsNullptrIfDataNotObject)
    {
        EXPECT_EQ(nullptr, parser.ParseFirs(nlohmann::json::array()));
        EXPECT_EQ(0, mockEventHandler->GetCallCount());
    }

    TEST_F(FlightInformationRegionDataParserTest, ItReturnsNullptrIfDataDoesNotContainFirs)
    {
        EXPECT_EQ(nullptr, parser.ParseFirs(nlohmann::json{{"not_flight_information_regions", "abc"}}));
        EXPECT_EQ(0, mockEventHandler->GetCallCount());
    }

    TEST_F(FlightInformationRegionDataParserTest, ItReturnsNullptrIfFirsNotArray)
    {
        EXPECT_EQ(nullptr, parser.ParseFirs(nlohmann::json{{"flight_information_regions", "abc"}}));
        EXPECT_EQ(0, mockEventHandler->GetCallCount());
    }

    TEST_F(FlightInformationRegionDataParserTest, ItParsesFirs)
    {
        const auto firData = nlohmann::json::array(
                {{
                         {"id", 1},
                         {"identifier", "EGTT"},
                         {"name", "London"},
                 },
                 {
                         {"id", 2},
                         {"identifier", "EGPX"},
                         {"name", "Scottish"},
                 }}
        );

        auto firs = parser.ParseFirs(nlohmann::json{{"flight_information_regions", firData}});
        EXPECT_EQ(2, firs->Count());

        const auto fir1 = firs->Get(1);
        EXPECT_NE(nullptr, fir1);
        EXPECT_EQ(1, fir1->Id());
        EXPECT_EQ("EGTT", fir1->Identifier());
        EXPECT_EQ("London", fir1->Name());

        const auto fir2 = firs->Get(2);
        EXPECT_NE(nullptr, fir2);
        EXPECT_EQ(2, fir2->Id());
        EXPECT_EQ("EGPX", fir2->Identifier());
        EXPECT_EQ("Scottish", fir2->Name());

        // Check event handler is called
        EXPECT_EQ(1, mockEventHandler->GetCallCount());
    }

    using BadFlightInformationRegionDataCheck = struct BadFlightInformationRegionDataCheck {
        // The name of the check
        std::string name;

        // The JSON for the test
        nlohmann::json data;
    };

    class FlightInformationRegionDataParserBadDataTest
        : public testing::TestWithParam<BadFlightInformationRegionDataCheck>
    {
        public:
        FlightInformationRegionDataParserBadDataTest()
            : mockEventHandler(std::make_shared<MockFlightInformationRegionsUpdatedEventHandler>(1)),
              eventBus(ECFMP::EventBus::MakeEventBus()),
              mockLogger(std::make_shared<testing::NiceMock<Log::MockLogger>>()), parser(mockLogger, eventBus)
        {
            // Add mock listener to event bus
            eventBus->SubscribeSync<ECFMP::Plugin::FlightInformationRegionsUpdatedEvent>(mockEventHandler);
        }

        std::shared_ptr<MockFlightInformationRegionsUpdatedEventHandler> mockEventHandler;
        std::shared_ptr<ECFMP::EventBus::InternalEventBus> eventBus;
        std::shared_ptr<testing::NiceMock<Log::MockLogger>> mockLogger;
        ECFMP::Api::FlightInformationRegionDataParser parser;
    };

    INSTANTIATE_TEST_SUITE_P(
            FlightInformationRegionDataParserBadDataTestCases, FlightInformationRegionDataParserBadDataTest,
            testing::Values(
                    BadFlightInformationRegionDataCheck{
                            "no_id",
                            {
                                    {"identifier", "EGPX"},
                                    {"name", "Scottish"},
                            }},
                    BadFlightInformationRegionDataCheck{
                            "id_not_integer",
                            {
                                    {"id", "abc"},
                                    {"identifier", "EGPX"},
                                    {"name", "Scottish"},
                            }},
                    BadFlightInformationRegionDataCheck{
                            "no_identifier",
                            {
                                    {"id", 2},
                                    {"name", "Scottish"},
                            }},
                    BadFlightInformationRegionDataCheck{
                            "identifier_not_string",
                            {
                                    {"id", 2},
                                    {"identifier", 123},
                                    {"name", "Scottish"},
                            }},
                    BadFlightInformationRegionDataCheck{"no_name", {{"id", 2}, {"identifier", "EGPX"}}},
                    BadFlightInformationRegionDataCheck{
                            "name_not_string",
                            {
                                    {"id", 2},
                                    {"identifier", "EGPX"},
                                    {"name", 123},
                            }}
            ),
            [](const ::testing::TestParamInfo<FlightInformationRegionDataParserBadDataTest::ParamType>& info) {
                return info.param.name;
            }
    );

    TEST_P(FlightInformationRegionDataParserBadDataTest, ItRejectsBadData)
    {
        const nlohmann::json validRegion = {
                {"id", 1},
                {"identifier", "EGTT"},
                {"name", "London"},
        };

        auto data = nlohmann::json::array({GetParam().data});
        data.push_back(validRegion);
        auto firs = parser.ParseFirs(nlohmann::json{{"flight_information_regions", data}});

        EXPECT_EQ(1, firs->Count());

        const auto fir1 = firs->Get(1);
        EXPECT_NE(nullptr, fir1);
        EXPECT_EQ(1, fir1->Id());
        EXPECT_EQ("EGTT", fir1->Identifier());
        EXPECT_EQ("London", fir1->Name());

        // Check event handler is called
        EXPECT_EQ(1, mockEventHandler->GetCallCount());
    }
}// namespace ECFMPTest::Api
