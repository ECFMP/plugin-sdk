#include "api/FlowMeasureFilterParser.h"
#include "api/ConcreteApiElementCollection.h"
#include "flow-sdk/Event.h"
#include "flow-sdk/EventFilter.h"
#include "flow-sdk/LevelRangeFilter.h"
#include "flow-sdk/RouteFilter.h"
#include "flowmeasure/ConcreteAirportFilter.h"
#include "flowmeasure/ConcreteFlowMeasureFilters.h"
#include "mock/EventMock.h"
#include "mock/MockLogger.h"
#include "nlohmann/json.hpp"

namespace FlowSdkTest::Api {

    class FlowMeasureFilterParserNoDataTest : public ::testing::Test
    {
        public:
        FlowMeasureFilterParserNoDataTest() : parser(std::make_shared<Log::MockLogger>())
        {}

        FlowSdk::Api::ConcreteApiElementCollection<FlowSdk::Event::Event> events;
        testing::NiceMock<Log::MockLogger> logger;
        FlowSdk::Api::FlowMeasureFilterParser parser;
    };

    TEST_F(FlowMeasureFilterParserNoDataTest, ItParsesNoData)
    {
        auto filters = parser.Parse(nlohmann::json::array(), events);
        EXPECT_NE(nullptr, filters);

        auto castedFilters = static_cast<const FlowSdk::FlowMeasure::ConcreteFlowMeasureFilters&>(*filters.get());

        ASSERT_EQ(castedFilters.AirportFilters().size(), 0);
        ASSERT_EQ(castedFilters.LevelFilters().size(), 0);
        ASSERT_EQ(castedFilters.MultipleLevelFilters().size(), 0);
        ASSERT_EQ(castedFilters.EventFilters().size(), 0);
        ASSERT_EQ(castedFilters.RouteFilters().size(), 0);
    }

    struct FlowMeasureFilterParserAirportFilterTestCase {
        std::string description;
        std::string filterType;
        std::set<std::string> airports;
    };

    class FlowMeasureFilterParserAirportFilterTest
        : public ::testing::TestWithParam<FlowMeasureFilterParserAirportFilterTestCase>
    {
        public:
        FlowMeasureFilterParserAirportFilterTest() : parser(std::make_shared<Log::MockLogger>())
        {}

        FlowSdk::Api::ConcreteApiElementCollection<FlowSdk::Event::Event> events;
        testing::NiceMock<Log::MockLogger> logger;
        FlowSdk::Api::FlowMeasureFilterParser parser;
    };

    TEST_P(FlowMeasureFilterParserAirportFilterTest, ItParsesAnAirportFilter)
    {
        auto filter = nlohmann::json{{"type", GetParam().filterType}, {"value", GetParam().airports}};
        auto filters = parser.Parse(nlohmann::json::array({filter}), events);
        EXPECT_NE(nullptr, filters);

        auto castedFilters = static_cast<const FlowSdk::FlowMeasure::ConcreteFlowMeasureFilters&>(*filters.get());

        ASSERT_EQ(castedFilters.AirportFilters().size(), 1);
        ASSERT_EQ(castedFilters.LevelFilters().size(), 0);
        ASSERT_EQ(castedFilters.MultipleLevelFilters().size(), 0);
        ASSERT_EQ(castedFilters.EventFilters().size(), 0);
        ASSERT_EQ(castedFilters.RouteFilters().size(), 0);

        auto expectedFilterType = GetParam().filterType == "ADES" ? FlowSdk::FlowMeasure::AirportFilterType::Destination
                                                                  : FlowSdk::FlowMeasure::AirportFilterType::Departure;
        const auto& airportFilter = castedFilters.AirportFilters().front();
        ASSERT_EQ(airportFilter->Type(), expectedFilterType);
        ASSERT_EQ(airportFilter->AirportStrings().size(), GetParam().airports.size());
        ASSERT_EQ(airportFilter->AirportStrings(), GetParam().airports);
    }

    INSTANTIATE_TEST_SUITE_P(
            FlowMeasureFilterParserAirportFilterTest, FlowMeasureFilterParserAirportFilterTest,
            testing::Values(
                    FlowMeasureFilterParserAirportFilterTestCase{"ADEP", "ADEP", {"EGLL", "EGKK", "EGSS"}},
                    FlowMeasureFilterParserAirportFilterTestCase{"ADES", "ADES", {"EGLL", "EGKK", "EGSS"}}
            ),
            [](const testing::TestParamInfo<FlowMeasureFilterParserAirportFilterTestCase>& info) {
                return info.param.description;
            }
    );

    struct FlowMeasureFilterParserLevelRangeTestCase {
        std::string description;
        std::string filterType;
        int jsonLevel;
        FlowSdk::FlowMeasure::LevelRangeFilterType expectedFilterType;
        int expectedLevel;
    };

    class FlowMeasureFilterParserLevelRangeFilterTest
        : public ::testing::TestWithParam<FlowMeasureFilterParserLevelRangeTestCase>
    {
        public:
        FlowMeasureFilterParserLevelRangeFilterTest() : parser(std::make_shared<Log::MockLogger>())
        {}

        FlowSdk::Api::ConcreteApiElementCollection<FlowSdk::Event::Event> events;
        testing::NiceMock<Log::MockLogger> logger;
        FlowSdk::Api::FlowMeasureFilterParser parser;
    };

    TEST_P(FlowMeasureFilterParserLevelRangeFilterTest, ItParsesALevelRangeFilter)
    {
        auto filter = nlohmann::json{{"type", GetParam().filterType}, {"value", GetParam().jsonLevel}};
        auto filters = parser.Parse(nlohmann::json::array({filter}), events);
        EXPECT_NE(nullptr, filters);

        auto castedFilters = static_cast<const FlowSdk::FlowMeasure::ConcreteFlowMeasureFilters&>(*filters.get());

        ASSERT_EQ(castedFilters.AirportFilters().size(), 0);
        ASSERT_EQ(castedFilters.LevelFilters().size(), 1);
        ASSERT_EQ(castedFilters.MultipleLevelFilters().size(), 0);
        ASSERT_EQ(castedFilters.EventFilters().size(), 0);
        ASSERT_EQ(castedFilters.RouteFilters().size(), 0);

        const auto& levelFilter = castedFilters.LevelFilters().front();
        ASSERT_EQ(GetParam().expectedFilterType, levelFilter->Type());
        ASSERT_EQ(GetParam().expectedLevel, levelFilter->Level());
    }

    INSTANTIATE_TEST_SUITE_P(
            FlowMeasureFilterParserLevelRangeFilterTest, FlowMeasureFilterParserLevelRangeFilterTest,
            testing::Values(
                    FlowMeasureFilterParserLevelRangeTestCase{
                            "level_above", "level_above", 100, FlowSdk::FlowMeasure::LevelRangeFilterType::AtOrAbove,
                            100},
                    FlowMeasureFilterParserLevelRangeTestCase{
                            "level_below", "level_below", 100, FlowSdk::FlowMeasure::LevelRangeFilterType::AtOrBelow,
                            100}
            ),
            [](const testing::TestParamInfo<FlowMeasureFilterParserLevelRangeTestCase>& info) {
                return info.param.description;
            }
    );

    struct FlowMeasureFilterParserSpecificLevelTestCase {
        std::string description;
        std::vector<int> jsonLevels;
        std::vector<int> expectedLevels;
    };

    class FlowMeasureFilterParserSpecificLevelFilterTest
        : public ::testing::TestWithParam<FlowMeasureFilterParserSpecificLevelTestCase>
    {
        public:
        FlowMeasureFilterParserSpecificLevelFilterTest() : parser(std::make_shared<Log::MockLogger>())
        {}

        FlowSdk::Api::ConcreteApiElementCollection<FlowSdk::Event::Event> events;
        testing::NiceMock<Log::MockLogger> logger;
        FlowSdk::Api::FlowMeasureFilterParser parser;
    };

    TEST_P(FlowMeasureFilterParserSpecificLevelFilterTest, ItParsesASpecificLevelFilter)
    {
        auto filter = nlohmann::json{{"type", "level"}, {"value", GetParam().jsonLevels}};
        auto filters = parser.Parse(nlohmann::json::array({filter}), events);
        EXPECT_NE(nullptr, filters);

        auto castedFilters = static_cast<const FlowSdk::FlowMeasure::ConcreteFlowMeasureFilters&>(*filters.get());

        ASSERT_EQ(castedFilters.AirportFilters().size(), 0);
        ASSERT_EQ(castedFilters.LevelFilters().size(), 0);
        ASSERT_EQ(castedFilters.MultipleLevelFilters().size(), 1);
        ASSERT_EQ(castedFilters.EventFilters().size(), 0);
        ASSERT_EQ(castedFilters.RouteFilters().size(), 0);

        const auto& levelFilter = castedFilters.MultipleLevelFilters().front();
        ASSERT_EQ(GetParam().expectedLevels, levelFilter->Levels());
    }

    INSTANTIATE_TEST_SUITE_P(
            FlowMeasureFilterParserSpecificLevelFilterTest, FlowMeasureFilterParserSpecificLevelFilterTest,
            testing::Values(FlowMeasureFilterParserSpecificLevelTestCase{
                    "at_level", std::vector<int>({100, 150}), std::vector<int>({100, 150})}),
            [](const testing::TestParamInfo<FlowMeasureFilterParserSpecificLevelTestCase>& info) {
                return info.param.description;
            }
    );

    struct FlowMeasureFilterParserEventParticipationTestCase {
        std::string description;
        std::string jsonFilterType;
        int jsonEventId;
        FlowSdk::FlowMeasure::EventParticipation expectedFilterType;
        int expectedEventId;
    };

    class FlowMeasureFilterParserEventParticipationFilterTest
        : public ::testing::TestWithParam<FlowMeasureFilterParserEventParticipationTestCase>
    {
        public:
        FlowMeasureFilterParserEventParticipationFilterTest() : parser(std::make_shared<Log::MockLogger>())
        {
            auto event1 = std::make_shared<FlowSdk::Mock::Event::EventMock>();
            ON_CALL(*event1, Id).WillByDefault(testing::Return(100));
            auto event2 = std::make_shared<FlowSdk::Mock::Event::EventMock>();
            ON_CALL(*event2, Id).WillByDefault(testing::Return(150));

            events.Add(event1);
            events.Add(event2);
        }

        FlowSdk::Api::ConcreteApiElementCollection<FlowSdk::Event::Event> events;
        testing::NiceMock<Log::MockLogger> logger;
        FlowSdk::Api::FlowMeasureFilterParser parser;
    };

    TEST_P(FlowMeasureFilterParserEventParticipationFilterTest, ItParsesAnEventFilter)
    {
        auto filter = nlohmann::json{{"type", GetParam().jsonFilterType}, {"value", GetParam().jsonEventId}};
        auto filters = parser.Parse(nlohmann::json::array({filter}), events);
        EXPECT_NE(nullptr, filters);

        auto castedFilters = static_cast<const FlowSdk::FlowMeasure::ConcreteFlowMeasureFilters&>(*filters.get());

        ASSERT_EQ(castedFilters.AirportFilters().size(), 0);
        ASSERT_EQ(castedFilters.LevelFilters().size(), 0);
        ASSERT_EQ(castedFilters.MultipleLevelFilters().size(), 0);
        ASSERT_EQ(castedFilters.EventFilters().size(), 1);
        ASSERT_EQ(castedFilters.RouteFilters().size(), 0);

        const auto& eventFilter = castedFilters.EventFilters().front();
        ASSERT_EQ(GetParam().expectedFilterType, eventFilter->Participation());
        ASSERT_EQ(GetParam().expectedEventId, eventFilter->Event().Id());
    }

    INSTANTIATE_TEST_SUITE_P(
            FlowMeasureFilterParserEventParticipationFilterTest, FlowMeasureFilterParserEventParticipationFilterTest,
            testing::Values(
                    FlowMeasureFilterParserEventParticipationTestCase{
                            "member_event", "member_event", 100,
                            FlowSdk::FlowMeasure::EventParticipation::Participating, 100},
                    FlowMeasureFilterParserEventParticipationTestCase{
                            "member_not_event", "member_not_event", 100,
                            FlowSdk::FlowMeasure::EventParticipation::NotParticipating, 100}
            ),
            [](const testing::TestParamInfo<FlowMeasureFilterParserEventParticipationTestCase>& info) {
                return info.param.description;
            }
    );

    struct FlowMeasureFilterParserRouteFilterTestCase {
        std::string description;
        std::set<std::string> jsonWaypoints;
        std::set<std::string> expectedWaypoints;
    };

    class FlowMeasureFilterParserRouteFilterTest
        : public ::testing::TestWithParam<FlowMeasureFilterParserRouteFilterTestCase>
    {
        public:
        FlowMeasureFilterParserRouteFilterTest() : parser(std::make_shared<Log::MockLogger>())
        {}

        FlowSdk::Api::ConcreteApiElementCollection<FlowSdk::Event::Event> events;
        testing::NiceMock<Log::MockLogger> logger;
        FlowSdk::Api::FlowMeasureFilterParser parser;
    };

    TEST_P(FlowMeasureFilterParserRouteFilterTest, ItParsesARouteFilter)
    {
        auto filter = nlohmann::json{{"type", "waypoint"}, {"value", GetParam().jsonWaypoints}};
        auto filters = parser.Parse(nlohmann::json::array({filter}), events);
        EXPECT_NE(nullptr, filters);

        auto castedFilters = static_cast<const FlowSdk::FlowMeasure::ConcreteFlowMeasureFilters&>(*filters.get());

        ASSERT_EQ(castedFilters.AirportFilters().size(), 0);
        ASSERT_EQ(castedFilters.LevelFilters().size(), 0);
        ASSERT_EQ(castedFilters.MultipleLevelFilters().size(), 0);
        ASSERT_EQ(castedFilters.EventFilters().size(), 0);
        ASSERT_EQ(castedFilters.RouteFilters().size(), 1);

        const auto& routeFilter = castedFilters.RouteFilters().front();
        ASSERT_EQ(GetParam().expectedWaypoints, routeFilter->RouteStrings());
    }

    INSTANTIATE_TEST_SUITE_P(
            FlowMeasureFilterParserRouteFilterTest, FlowMeasureFilterParserRouteFilterTest,
            testing::Values(
                    FlowMeasureFilterParserRouteFilterTestCase{"single_waypoint", {"A"}, {"A"}},
                    FlowMeasureFilterParserRouteFilterTestCase{"multiple_waypoints", {"A", "B"}, {"A", "B"}}
            ),
            [](const testing::TestParamInfo<FlowMeasureFilterParserRouteFilterTestCase>& info) {
                return info.param.description;
            }
    );

    struct FlowMeasureFilterParserInvalidDataTestCase {
        std::string description;
        nlohmann::json filterData;
    };

    class FlowMeasureFilterParserInvalidDataTest
        : public ::testing::TestWithParam<FlowMeasureFilterParserInvalidDataTestCase>
    {
        public:
        FlowMeasureFilterParserInvalidDataTest() : parser(std::make_shared<Log::MockLogger>())
        {
            auto event1 = std::make_shared<FlowSdk::Mock::Event::EventMock>();
            ON_CALL(*event1, Id()).WillByDefault(testing::Return(100));
            auto event2 = std::make_shared<FlowSdk::Mock::Event::EventMock>();
            ON_CALL(*event2, Id()).WillByDefault(testing::Return(150));

            events.Add(event1);
            events.Add(event2);
        }

        FlowSdk::Api::ConcreteApiElementCollection<FlowSdk::Event::Event> events;
        testing::NiceMock<Log::MockLogger> logger;
        FlowSdk::Api::FlowMeasureFilterParser parser;
    };

    TEST_P(FlowMeasureFilterParserInvalidDataTest, ItReturnsNullptrOnBadData)
    {
        auto filters = parser.Parse(GetParam().filterData, events);
        EXPECT_EQ(nullptr, filters);
    }

    INSTANTIATE_TEST_SUITE_P(
            FlowMeasureFilterParserInvalidDataTest, FlowMeasureFilterParserInvalidDataTest,
            testing::Values(
                    FlowMeasureFilterParserInvalidDataTestCase{"json_is_not_array", nlohmann::json{}},
                    FlowMeasureFilterParserInvalidDataTestCase{
                            "filter_has_no_value_field", nlohmann::json::array({{{"type", "waypoint"}}})},
                    FlowMeasureFilterParserInvalidDataTestCase{
                            "filter_has_no_type_field", nlohmann::json::array({{{"value", {"A"}}}})},
                    FlowMeasureFilterParserInvalidDataTestCase{
                            "filter_type_field_not_string", nlohmann::json::array({{{"type", 1}, {"value", {"A"}}}})},
                    FlowMeasureFilterParserInvalidDataTestCase{
                            "filter_is_adep_but_value_is_not_array",
                            nlohmann::json::array({{{"type", "ADEP"}, {"value", 1}}})},
                    FlowMeasureFilterParserInvalidDataTestCase{
                            "filter_is_adep_but_value_array_has_non_string_item",
                            nlohmann::json::array({{{"type", "ADEP"}, {"value", {1}}}})},
                    FlowMeasureFilterParserInvalidDataTestCase{
                            "filter_is_ades_but_value_is_not_array",
                            nlohmann::json::array({{{"type", "ADES"}, {"value", 1}}})},
                    FlowMeasureFilterParserInvalidDataTestCase{
                            "filter_is_ades_but_value_array_has_non_string_item",
                            nlohmann::json::array({{{"type", "ADES"}, {"value", {1}}}})},
                    FlowMeasureFilterParserInvalidDataTestCase{
                            "filter_is_level_above_but_value_is_not_integer",
                            nlohmann::json::array({{{"type", "level_above"}, {"value", "A"}}})},
                    FlowMeasureFilterParserInvalidDataTestCase{
                            "filter_is_level_below_but_value_is_not_integer",
                            nlohmann::json::array({{{"type", "level_below"}, {"value", "A"}}})},
                    FlowMeasureFilterParserInvalidDataTestCase{
                            "filter_is_level_but_value_is_not_array",
                            nlohmann::json::array({{{"type", "level"}, {"value", "A"}}})},
                    FlowMeasureFilterParserInvalidDataTestCase{
                            "filter_is_level_but_values_are_not_integers",
                            nlohmann::json::array({{{"type", "level"}, {"value", nlohmann::json::array({"A"})}}})},
                    FlowMeasureFilterParserInvalidDataTestCase{
                            "filter_is_member_event_but_value_is_not_integer",
                            nlohmann::json::array({{{"type", "member_event"}, {"value", "A"}}})},
                    FlowMeasureFilterParserInvalidDataTestCase{
                            "filter_is_member_event_but_event_not_found",
                            nlohmann::json::array({{{"type", "member_event"}, {"value", 200}}})},
                    FlowMeasureFilterParserInvalidDataTestCase{
                            "filter_is_member_not_event_but_value_is_not_integer",
                            nlohmann::json::array({{{"type", "member_not_event"}, {"value", "A"}}})},
                    FlowMeasureFilterParserInvalidDataTestCase{
                            "filter_is_member_not_event_but_event_not_found",
                            nlohmann::json::array({{{"type", "member_not_event"}, {"value", 200}}})},
                    FlowMeasureFilterParserInvalidDataTestCase{
                            "filter_is_waypoint_but_value_is_not_array",
                            nlohmann::json::array({{{"type", "waypoint"}, {"value", 1}}})},
                    FlowMeasureFilterParserInvalidDataTestCase{
                            "filter_is_waypoint_but_value_array_has_non_string_item",
                            nlohmann::json::array({{{"type", "waypoint"}, {"value", {1}}}})}

            ),
            [](const testing::TestParamInfo<FlowMeasureFilterParserInvalidDataTestCase>& info) {
                return info.param.description;
            }
    );
}// namespace FlowSdkTest::Api
