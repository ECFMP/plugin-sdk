#include "api/EventDataParser.h"
#include "ECFMP/flightinformationregion/FlightInformationRegion.h"
#include "api/InternalElementCollectionTypes.h"
#include "date/ParseDateStrings.h"
#include "flightinformationregion/ConcreteFlightInformationRegion.h"
#include "mock/MockLogger.h"
#include "nlohmann/json.hpp"
#include "nlohmann/json_fwd.hpp"

namespace ECFMPTest::Api {
    class EventDataParserTest : public testing::Test
    {
        public:
        EventDataParserTest() : mockLogger(std::make_shared<testing::NiceMock<Log::MockLogger>>()), parser(mockLogger)
        {
            firs.Add(std::make_shared<ECFMP::FlightInformationRegion::ConcreteFlightInformationRegion>(
                    1, "EGTT", "London"
            ));
            firs.Add(std::make_shared<ECFMP::FlightInformationRegion::ConcreteFlightInformationRegion>(
                    2, "EGPX", "Scottish"
            ));
        }

        ECFMP::Api::InternalFlightInformationRegionCollection firs;
        std::shared_ptr<testing::NiceMock<Log::MockLogger>> mockLogger;
        ECFMP::Api::EventDataParser parser;
    };

    TEST_F(EventDataParserTest, ItReturnsNullptrIfDataNotObject)
    {
        EXPECT_EQ(nullptr, parser.ParseEvents(nlohmann::json::array(), firs));
    }

    TEST_F(EventDataParserTest, ItReturnsNullptrIfIfDataDoesNotContainEvents)
    {
        EXPECT_EQ(nullptr, parser.ParseEvents(nlohmann::json{{"not_events", "abc"}}, firs));
    }

    TEST_F(EventDataParserTest, ItReturnsNullptrIfIfEventsNotArray)
    {
        EXPECT_EQ(nullptr, parser.ParseEvents(nlohmann::json{{"events", "abc"}}, firs));
    }

    TEST_F(EventDataParserTest, ItParsesEvents)
    {
        const auto eventData = nlohmann::json::array(
                {{{"id", 1},
                  {"flight_information_region_id", 1},
                  {"name", "London Event"},
                  {"date_start", "2022-04-16T13:16:00Z"},
                  {"date_end", "2022-04-16T13:17:00Z"},
                  {"vatcan_code", "abc"},
                  {"participants",
                   nlohmann::json::array(
                           {{
                                    {"cid", 1203533},
                                    {"origin", "EGKK"},
                                    {"destination", "EGLL"},
                            },
                            {
                                    {"cid", 1203534},
                                    {"origin", "EGKK"},
                                    {"destination", "EGLL"},
                            }}
                   )}},
                 {{"id", 2},
                  {"flight_information_region_id", 2},
                  {"name", "Scottish Event"},
                  {"date_start", "2022-04-17T13:16:00Z"},
                  {"date_end", "2022-04-18T13:17:00Z"},
                  {"vatcan_code", "def"},
                  {"participants",
                   nlohmann::json::array(
                           {{
                                    {"cid", 1203535},
                                    {"origin", "EGLL"},
                                    {"destination", "EGCC"},
                            },
                            {
                                    {"cid", 1203536},
                                    {"origin", "EGCC"},
                                    {"destination", "EGPH"},
                            }}
                   )}}}
        );

        auto events = parser.ParseEvents(nlohmann::json{{"events", eventData}}, firs);
        EXPECT_EQ(2, events->Count());

        const auto event1 = events->Get(1);
        EXPECT_NE(nullptr, event1);
        EXPECT_EQ(1, event1->Id());
        EXPECT_EQ("London Event", event1->Name());
        EXPECT_EQ(1, event1->FlightInformationRegion().Id());
        EXPECT_EQ(ECFMP::Date::TimePointFromDateString("2022-04-16T13:16:00Z"), event1->Start());
        EXPECT_EQ(ECFMP::Date::TimePointFromDateString("2022-04-16T13:17:00Z"), event1->End());
        EXPECT_EQ("abc", event1->VatcanCode());
        auto event1Participants = event1->Participants();
        EXPECT_EQ(2, event1Participants.size());
        const auto event1Participant1 = event1Participants[0];
        EXPECT_EQ(1203533, event1Participant1->Cid());
        EXPECT_EQ("EGKK", event1Participant1->OriginAirport());
        EXPECT_EQ("EGLL", event1Participant1->DestinationAirport());
        const auto event1Participant2 = event1Participants[1];
        EXPECT_EQ(1203534, event1Participant2->Cid());
        EXPECT_EQ("EGKK", event1Participant2->OriginAirport());
        EXPECT_EQ("EGLL", event1Participant2->DestinationAirport());

        const auto event2 = events->Get(2);
        EXPECT_EQ(2, event2->Id());
        EXPECT_EQ("Scottish Event", event2->Name());
        EXPECT_EQ(2, event2->FlightInformationRegion().Id());
        EXPECT_EQ(ECFMP::Date::TimePointFromDateString("2022-04-17T13:16:00Z"), event2->Start());
        EXPECT_EQ(ECFMP::Date::TimePointFromDateString("2022-04-18T13:17:00Z"), event2->End());
        EXPECT_EQ("def", event2->VatcanCode());
        auto event2Participants = event2->Participants();
        EXPECT_EQ(2, event2Participants.size());
        const auto event2Participant1 = event2Participants[0];
        EXPECT_EQ(1203535, event2Participant1->Cid());
        EXPECT_EQ("EGLL", event2Participant1->OriginAirport());
        EXPECT_EQ("EGCC", event2Participant1->DestinationAirport());
        const auto event2Participant2 = event2Participants[1];
        EXPECT_EQ(1203536, event2Participant2->Cid());
        EXPECT_EQ("EGCC", event2Participant2->OriginAirport());
        EXPECT_EQ("EGPH", event2Participant2->DestinationAirport());
    }

    TEST_F(EventDataParserTest, ItParsesEventsWithNoParticipants)
    {
        const auto eventData = nlohmann::json::array(
                {{{"id", 1},
                  {"flight_information_region_id", 1},
                  {"name", "London Event"},
                  {"date_start", "2022-04-16T13:16:00Z"},
                  {"date_end", "2022-04-16T13:17:00Z"},
                  {"vatcan_code", "abc"},
                  {"participants", nlohmann::json::array()}},
                 {{"id", 2},
                  {"flight_information_region_id", 2},
                  {"name", "Scottish Event"},
                  {"date_start", "2022-04-17T13:16:00Z"},
                  {"date_end", "2022-04-18T13:17:00Z"},
                  {"vatcan_code", "def"},
                  {"participants", nlohmann::json::array()}}}
        );

        auto events = parser.ParseEvents(nlohmann::json{{"events", eventData}}, firs);
        EXPECT_EQ(2, events->Count());

        const auto event1 = events->Get(1);
        EXPECT_NE(nullptr, event1);
        EXPECT_EQ(1, event1->Id());
        EXPECT_EQ("London Event", event1->Name());
        EXPECT_EQ(1, event1->FlightInformationRegion().Id());
        EXPECT_EQ(ECFMP::Date::TimePointFromDateString("2022-04-16T13:16:00Z"), event1->Start());
        EXPECT_EQ(ECFMP::Date::TimePointFromDateString("2022-04-16T13:17:00Z"), event1->End());
        EXPECT_EQ("abc", event1->VatcanCode());

        const auto event2 = events->Get(2);
        EXPECT_EQ(2, event2->Id());
        EXPECT_EQ("Scottish Event", event2->Name());
        EXPECT_EQ(2, event2->FlightInformationRegion().Id());
        EXPECT_EQ(ECFMP::Date::TimePointFromDateString("2022-04-17T13:16:00Z"), event2->Start());
        EXPECT_EQ(ECFMP::Date::TimePointFromDateString("2022-04-18T13:17:00Z"), event2->End());
        EXPECT_EQ("def", event2->VatcanCode());
    }

    TEST_F(EventDataParserTest, ItParsesEventsWithNoVatcanCode)
    {
        const auto eventData = nlohmann::json::array(
                {{{"id", 1},
                  {"flight_information_region_id", 1},
                  {"name", "London Event"},
                  {"date_start", "2022-04-16T13:16:00Z"},
                  {"date_end", "2022-04-16T13:17:00Z"},
                  {"vatcan_code", nlohmann::json::value_t::null},
                  {"participants",
                   nlohmann::json::array(
                           {{
                                    {"cid", 1203533},
                                    {"origin", "EGKK"},
                                    {"destination", "EGLL"},
                            },
                            {
                                    {"cid", 1203534},
                                    {"origin", "EGKK"},
                                    {"destination", "EGLL"},
                            }}
                   )}},
                 {{"id", 2},
                  {"flight_information_region_id", 2},
                  {"name", "Scottish Event"},
                  {"date_start", "2022-04-17T13:16:00Z"},
                  {"date_end", "2022-04-18T13:17:00Z"},
                  {"vatcan_code", nlohmann::json::value_t::null},
                  {"participants",
                   nlohmann::json::array(
                           {{
                                    {"cid", 1203535},
                                    {"origin", "EGLL"},
                                    {"destination", "EGCC"},
                            },
                            {
                                    {"cid", 1203536},
                                    {"origin", "EGCC"},
                                    {"destination", "EGPH"},
                            }}
                   )}}}
        );

        auto events = parser.ParseEvents(nlohmann::json{{"events", eventData}}, firs);
        EXPECT_EQ(2, events->Count());

        const auto event1 = events->Get(1);
        EXPECT_NE(nullptr, event1);
        EXPECT_EQ(1, event1->Id());
        EXPECT_EQ("London Event", event1->Name());
        EXPECT_EQ(1, event1->FlightInformationRegion().Id());
        EXPECT_EQ(ECFMP::Date::TimePointFromDateString("2022-04-16T13:16:00Z"), event1->Start());
        EXPECT_EQ(ECFMP::Date::TimePointFromDateString("2022-04-16T13:17:00Z"), event1->End());
        EXPECT_EQ("", event1->VatcanCode());

        const auto event2 = events->Get(2);
        EXPECT_EQ(2, event2->Id());
        EXPECT_EQ("Scottish Event", event2->Name());
        EXPECT_EQ(2, event2->FlightInformationRegion().Id());
        EXPECT_EQ(ECFMP::Date::TimePointFromDateString("2022-04-17T13:16:00Z"), event2->Start());
        EXPECT_EQ(ECFMP::Date::TimePointFromDateString("2022-04-18T13:17:00Z"), event2->End());
        EXPECT_EQ("", event2->VatcanCode());
    }

    TEST_F(EventDataParserTest, ItParsesEventsWithNoParticipantOriginOrDestination)
    {
        // TODO: Add event participation and fix this test
        const auto eventData = nlohmann::json::array(
                {{{"id", 1},
                  {"flight_information_region_id", 1},
                  {"name", "London Event"},
                  {"date_start", "2022-04-16T13:16:00Z"},
                  {"date_end", "2022-04-16T13:17:00Z"},
                  {"vatcan_code", "abc"},
                  {"participants",
                   nlohmann::json::array(
                           {{
                                    {"cid", 1203533},
                                    {"origin", nlohmann::json::value_t::null},
                                    {"destination", nlohmann::json::value_t::null},
                            },
                            {
                                    {"cid", 1203534},
                                    {"origin", nlohmann::json::value_t::null},
                                    {"destination", nlohmann::json::value_t::null},
                            }}
                   )}},
                 {{"id", 2},
                  {"flight_information_region_id", 2},
                  {"name", "Scottish Event"},
                  {"date_start", "2022-04-17T13:16:00Z"},
                  {"date_end", "2022-04-18T13:17:00Z"},
                  {"vatcan_code", "def"},
                  {"participants",
                   nlohmann::json::array(
                           {{
                                    {"cid", 1203535},
                                    {"origin", nlohmann::json::value_t::null},
                                    {"destination", nlohmann::json::value_t::null},
                            },
                            {
                                    {"cid", 1203536},
                                    {"origin", nlohmann::json::value_t::null},
                                    {"destination", nlohmann::json::value_t::null},
                            }}
                   )}}}
        );

        auto events = parser.ParseEvents(nlohmann::json{{"events", eventData}}, firs);
        EXPECT_EQ(2, events->Count());

        const auto event1 = events->Get(1);
        EXPECT_NE(nullptr, event1);
        EXPECT_EQ(1, event1->Id());
        EXPECT_EQ("London Event", event1->Name());
        EXPECT_EQ(1, event1->FlightInformationRegion().Id());
        EXPECT_EQ(ECFMP::Date::TimePointFromDateString("2022-04-16T13:16:00Z"), event1->Start());
        EXPECT_EQ(ECFMP::Date::TimePointFromDateString("2022-04-16T13:17:00Z"), event1->End());
        EXPECT_EQ("abc", event1->VatcanCode());
        auto event1Participants = event1->Participants();
        EXPECT_EQ(2, event1Participants.size());
        EXPECT_EQ(1203533, event1Participants[0]->Cid());
        EXPECT_EQ("", event1Participants[0]->OriginAirport());
        EXPECT_EQ("", event1Participants[0]->DestinationAirport());
        EXPECT_EQ(1203534, event1Participants[1]->Cid());
        EXPECT_EQ("", event1Participants[1]->OriginAirport());
        EXPECT_EQ("", event1Participants[1]->DestinationAirport());

        const auto event2 = events->Get(2);
        EXPECT_EQ(2, event2->Id());
        EXPECT_EQ("Scottish Event", event2->Name());
        EXPECT_EQ(2, event2->FlightInformationRegion().Id());
        EXPECT_EQ(ECFMP::Date::TimePointFromDateString("2022-04-17T13:16:00Z"), event2->Start());
        EXPECT_EQ(ECFMP::Date::TimePointFromDateString("2022-04-18T13:17:00Z"), event2->End());
        EXPECT_EQ("def", event2->VatcanCode());
        auto event2Participants = event2->Participants();
        EXPECT_EQ(2, event2Participants.size());
        EXPECT_EQ(1203535, event2Participants[0]->Cid());
        EXPECT_EQ("", event2Participants[0]->OriginAirport());
        EXPECT_EQ("", event2Participants[0]->DestinationAirport());
        EXPECT_EQ(1203536, event2Participants[1]->Cid());
        EXPECT_EQ("", event2Participants[1]->OriginAirport());
        EXPECT_EQ("", event2Participants[1]->DestinationAirport());
    }

    using BadEventDataCheck = struct BadEventDataCheck {
        // The name of the check
        std::string name;

        // The JSON for the test
        nlohmann::json data;
    };

    class EventDataParserBadDataTest : public testing::TestWithParam<BadEventDataCheck>
    {
        public:
        EventDataParserBadDataTest()
            : mockLogger(std::make_shared<testing::NiceMock<Log::MockLogger>>()), parser(mockLogger)
        {
            firs.Add(std::make_shared<ECFMP::FlightInformationRegion::ConcreteFlightInformationRegion>(
                    1, "EGTT", "London"
            ));
            firs.Add(std::make_shared<ECFMP::FlightInformationRegion::ConcreteFlightInformationRegion>(
                    2, "EGPX", "Scottish"
            ));
        }

        ECFMP::Api::InternalFlightInformationRegionCollection firs;
        std::shared_ptr<testing::NiceMock<Log::MockLogger>> mockLogger;
        ECFMP::Api::EventDataParser parser;
    };

    INSTANTIATE_TEST_SUITE_P(
            EventDataParserBadDataTestCases, EventDataParserBadDataTest,
            testing::Values(
                    BadEventDataCheck{
                            "no_id",
                            {{"flight_information_region_id", 1},
                             {"name", "London Event"},
                             {"date_start", "2022-04-16T13:16:00Z"},
                             {"date_end", "2022-04-16T13:17:00Z"},
                             {"vatcan_code", "abc"},
                             {"participants",
                              nlohmann::json::array(
                                      {{
                                               {"cid", 1203533},
                                               {"origin", "EGKK"},
                                               {"destination", "EGLL"},
                                       },
                                       {
                                               {"cid", 1203534},
                                               {"origin", "EGKK"},
                                               {"destination", "EGLL"},
                                       }}
                              )}}},
                    BadEventDataCheck{
                            "id_not_integer",
                            {{"id", "abc"},
                             {"flight_information_region_id", 1},
                             {"name", "London Event"},
                             {"date_start", "2022-04-16T13:16:00Z"},
                             {"date_end", "2022-04-16T13:17:00Z"},
                             {"vatcan_code", "abc"},
                             {"participants",
                              nlohmann::json::array(
                                      {{
                                               {"cid", 1203533},
                                               {"origin", "EGKK"},
                                               {"destination", "EGLL"},
                                       },
                                       {
                                               {"cid", 1203534},
                                               {"origin", "EGKK"},
                                               {"destination", "EGLL"},
                                       }}
                              )}}},
                    BadEventDataCheck{
                            "no_flight_information_region",
                            {{"id", 2},
                             {"name", "London Event"},
                             {"date_start", "2022-04-16T13:16:00Z"},
                             {"date_end", "2022-04-16T13:17:00Z"},
                             {"vatcan_code", "abc"},
                             {"participants",
                              nlohmann::json::array(
                                      {{
                                               {"cid", 1203533},
                                               {"origin", "EGKK"},
                                               {"destination", "EGLL"},
                                       },
                                       {
                                               {"cid", 1203534},
                                               {"origin", "EGKK"},
                                               {"destination", "EGLL"},
                                       }}
                              )}}},
                    BadEventDataCheck{
                            "flight_information_region_not_int",
                            {{"id", 2},
                             {"flight_information_region_id", "abc"},
                             {"name", "London Event"},
                             {"date_start", "2022-04-16T13:16:00Z"},
                             {"date_end", "2022-04-16T13:17:00Z"},
                             {"vatcan_code", "abc"},
                             {"participants",
                              nlohmann::json::array(
                                      {{
                                               {"cid", 1203533},
                                               {"origin", "EGKK"},
                                               {"destination", "EGLL"},
                                       },
                                       {
                                               {"cid", 1203534},
                                               {"origin", "EGKK"},
                                               {"destination", "EGLL"},
                                       }}
                              )}}},
                    BadEventDataCheck{
                            "flight_information_region_not_valid",
                            {{"id", 2},
                             {"flight_information_region_id", 55},
                             {"name", "London Event"},
                             {"date_start", "2022-04-16T13:16:00Z"},
                             {"date_end", "2022-04-16T13:17:00Z"},
                             {"vatcan_code", "abc"},
                             {"participants",
                              nlohmann::json::array(
                                      {{
                                               {"cid", 1203533},
                                               {"origin", "EGKK"},
                                               {"destination", "EGLL"},
                                       },
                                       {
                                               {"cid", 1203534},
                                               {"origin", "EGKK"},
                                               {"destination", "EGLL"},
                                       }}
                              )}}},
                    BadEventDataCheck{
                            "no_name",
                            {{"id", 2},
                             {"flight_information_region_id", 1},
                             {"date_start", "2022-04-16T13:16:00Z"},
                             {"date_end", "2022-04-16T13:17:00Z"},
                             {"vatcan_code", "abc"},
                             {"participants",
                              nlohmann::json::array(
                                      {{
                                               {"cid", 1203533},
                                               {"origin", "EGKK"},
                                               {"destination", "EGLL"},
                                       },
                                       {
                                               {"cid", 1203534},
                                               {"origin", "EGKK"},
                                               {"destination", "EGLL"},
                                       }}
                              )}}},
                    BadEventDataCheck{
                            "name_not_string",
                            {{"id", 2},
                             {"flight_information_region_id", 1},
                             {"name", 123},
                             {"date_start", "2022-04-16T13:16:00Z"},
                             {"date_end", "2022-04-16T13:17:00Z"},
                             {"vatcan_code", "abc"},
                             {"participants",
                              nlohmann::json::array(
                                      {{
                                               {"cid", 1203533},
                                               {"origin", "EGKK"},
                                               {"destination", "EGLL"},
                                       },
                                       {
                                               {"cid", 1203534},
                                               {"origin", "EGKK"},
                                               {"destination", "EGLL"},
                                       }}
                              )}}},
                    BadEventDataCheck{
                            "no_date_start",
                            {{"id", 2},
                             {"flight_information_region_id", 1},
                             {"name", "London Event"},
                             {"date_end", "2022-04-16T13:17:00Z"},
                             {"vatcan_code", "abc"},
                             {"participants",
                              nlohmann::json::array(
                                      {{
                                               {"cid", 1203533},
                                               {"origin", "EGKK"},
                                               {"destination", "EGLL"},
                                       },
                                       {
                                               {"cid", 1203534},
                                               {"origin", "EGKK"},
                                               {"destination", "EGLL"},
                                       }}
                              )}}},
                    BadEventDataCheck{
                            "date_start_not_string",
                            {{"id", 2},
                             {"flight_information_region_id", 1},
                             {"name", "London Event"},
                             {"date_start", 123},
                             {"date_end", "2022-04-16T13:17:00Z"},
                             {"vatcan_code", "abc"},
                             {"participants",
                              nlohmann::json::array(
                                      {{
                                               {"cid", 1203533},
                                               {"origin", "EGKK"},
                                               {"destination", "EGLL"},
                                       },
                                       {
                                               {"cid", 1203534},
                                               {"origin", "EGKK"},
                                               {"destination", "EGLL"},
                                       }}
                              )}}},
                    BadEventDataCheck{
                            "date_start_not_valid",
                            {{"id", 2},
                             {"flight_information_region_id", 1},
                             {"name", "London Event"},
                             {"date_start", "abc"},
                             {"date_end", "2022-04-16T13:17:00Z"},
                             {"vatcan_code", "abc"},
                             {"participants",
                              nlohmann::json::array(
                                      {{
                                               {"cid", 1203533},
                                               {"origin", "EGKK"},
                                               {"destination", "EGLL"},
                                       },
                                       {
                                               {"cid", 1203534},
                                               {"origin", "EGKK"},
                                               {"destination", "EGLL"},
                                       }}
                              )}}},
                    BadEventDataCheck{
                            "no_date_end",
                            {{"id", 2},
                             {"flight_information_region_id", 1},
                             {"name", "London Event"},
                             {"date_start", "2022-04-16T13:16:00Z"},
                             {"vatcan_code", "abc"},
                             {"participants",
                              nlohmann::json::array(
                                      {{
                                               {"cid", 1203533},
                                               {"origin", "EGKK"},
                                               {"destination", "EGLL"},
                                       },
                                       {
                                               {"cid", 1203534},
                                               {"origin", "EGKK"},
                                               {"destination", "EGLL"},
                                       }}
                              )}}},
                    BadEventDataCheck{
                            "date_end_not_string",
                            {{"id", 2},
                             {"flight_information_region_id", 1},
                             {"name", "London Event"},
                             {"date_start", "2022-04-16T13:16:00Z"},
                             {"date_end", 123},
                             {"vatcan_code", "abc"},
                             {"participants",
                              nlohmann::json::array(
                                      {{
                                               {"cid", 1203533},
                                               {"origin", "EGKK"},
                                               {"destination", "EGLL"},
                                       },
                                       {
                                               {"cid", 1203534},
                                               {"origin", "EGKK"},
                                               {"destination", "EGLL"},
                                       }}
                              )}}},
                    BadEventDataCheck{
                            "date_end_not_valid",
                            {{"id", 2},
                             {"flight_information_region_id", 1},
                             {"name", "London Event"},
                             {"date_start", "2022-04-16T13:16:00Z"},
                             {"date_end", "abc"},
                             {"vatcan_code", "abc"},
                             {"participants",
                              nlohmann::json::array(
                                      {{
                                               {"cid", 1203533},
                                               {"origin", "EGKK"},
                                               {"destination", "EGLL"},
                                       },
                                       {
                                               {"cid", 1203534},
                                               {"origin", "EGKK"},
                                               {"destination", "EGLL"},
                                       }}
                              )}}},
                    BadEventDataCheck{
                            "no_vatcan_code",
                            {{"id", 2},
                             {"flight_information_region_id", 1},
                             {"name", "London Event"},
                             {"date_start", "2022-04-16T13:16:00Z"},
                             {"date_end", "2022-04-16T13:17:00Z"},
                             {"participants",
                              nlohmann::json::array(
                                      {{
                                               {"cid", 1203533},
                                               {"origin", "EGKK"},
                                               {"destination", "EGLL"},
                                       },
                                       {
                                               {"cid", 1203534},
                                               {"origin", "EGKK"},
                                               {"destination", "EGLL"},
                                       }}
                              )}}},
                    BadEventDataCheck{
                            "vatcan_code_not_string",
                            {{"id", 2},
                             {"flight_information_region_id", 1},
                             {"name", "London Event"},
                             {"date_start", "2022-04-16T13:16:00Z"},
                             {"date_end", "2022-04-16T13:17:00Z"},
                             {"vatcan_code", 123},
                             {"participants",
                              nlohmann::json::array(
                                      {{
                                               {"cid", 1203533},
                                               {"origin", "EGKK"},
                                               {"destination", "EGLL"},
                                       },
                                       {
                                               {"cid", 1203534},
                                               {"origin", "EGKK"},
                                               {"destination", "EGLL"},
                                       }}
                              )}}},
                    BadEventDataCheck{
                            "no_participants",
                            {
                                    {"id", 2},
                                    {"flight_information_region_id", 1},
                                    {"name", "London Event"},
                                    {"date_start", "2022-04-16T13:16:00Z"},
                                    {"date_end", "2022-04-16T13:17:00Z"},
                                    {"vatcan_code", "abc"},
                            }},
                    BadEventDataCheck{
                            "participants_not_array",
                            {{"id", 2},
                             {"flight_information_region_id", 1},
                             {"name", "London Event"},
                             {"date_start", "2022-04-16T13:16:00Z"},
                             {"date_end", "2022-04-16T13:17:00Z"},
                             {"vatcan_code", "abc"},
                             {"participants", 123}}},
                    BadEventDataCheck{
                            "participant_cid_missing",
                            {{"id", 2},
                             {"flight_information_region_id", 1},
                             {"name", "London Event"},
                             {"date_start", "2022-04-16T13:16:00Z"},
                             {"date_end", "2022-04-16T13:17:00Z"},
                             {"vatcan_code", "abc"},
                             {"participants",
                              nlohmann::json::array(
                                      {{
                                               {"origin", "EGKK"},
                                               {"destination", "EGLL"},
                                       },
                                       {
                                               {"cid", 1203534},
                                               {"origin", "EGKK"},
                                               {"destination", "EGLL"},
                                       }}
                              )}}},
                    BadEventDataCheck{
                            "participant_cid_not_integer",
                            {{"id", 2},
                             {"flight_information_region_id", 1},
                             {"name", "London Event"},
                             {"date_start", "2022-04-16T13:16:00Z"},
                             {"date_end", "2022-04-16T13:17:00Z"},
                             {"vatcan_code", "abc"},
                             {"participants",
                              nlohmann::json::array(
                                      {{
                                               {"cid", "abc"},
                                               {"origin", "EGKK"},
                                               {"destination", "EGLL"},
                                       },
                                       {
                                               {"cid", 1203534},
                                               {"origin", "EGKK"},
                                               {"destination", "EGLL"},
                                       }}
                              )}}},
                    BadEventDataCheck{
                            "participant_origin_missing",
                            {{"id", 2},
                             {"flight_information_region_id", 1},
                             {"name", "London Event"},
                             {"date_start", "2022-04-16T13:16:00Z"},
                             {"date_end", "2022-04-16T13:17:00Z"},
                             {"vatcan_code", "abc"},
                             {"participants",
                              nlohmann::json::array(
                                      {{
                                               {"cid", 1203533},
                                               {"destination", "EGLL"},
                                       },
                                       {
                                               {"cid", 1203534},
                                               {"origin", "EGKK"},
                                               {"destination", "EGLL"},
                                       }}
                              )}}},
                    BadEventDataCheck{
                            "participant_origin_not_string",
                            {{"id", 2},
                             {"flight_information_region_id", 1},
                             {"name", "London Event"},
                             {"date_start", "2022-04-16T13:16:00Z"},
                             {"date_end", "2022-04-16T13:17:00Z"},
                             {"vatcan_code", "abc"},
                             {"participants",
                              nlohmann::json::array(
                                      {{
                                               {"cid", 1203533},
                                               {"origin", 123},
                                               {"destination", "EGLL"},
                                       },
                                       {
                                               {"cid", 1203534},
                                               {"origin", "EGKK"},
                                               {"destination", "EGLL"},
                                       }}
                              )}}},
                    BadEventDataCheck{
                            "participant_destination_missing",
                            {{"id", 2},
                             {"flight_information_region_id", 1},
                             {"name", "London Event"},
                             {"date_start", "2022-04-16T13:16:00Z"},
                             {"date_end", "2022-04-16T13:17:00Z"},
                             {"vatcan_code", "abc"},
                             {"participants",
                              nlohmann::json::array(
                                      {{
                                               {"cid", 1203533},
                                               {"origin", "EGKK"},
                                       },
                                       {
                                               {"cid", 1203534},
                                               {"origin", "EGKK"},
                                               {"destination", "EGLL"},
                                       }}
                              )}}},
                    BadEventDataCheck{
                            "participant_destination_not_string",
                            {{"id", 2},
                             {"flight_information_region_id", 1},
                             {"name", "London Event"},
                             {"date_start", "2022-04-16T13:16:00Z"},
                             {"date_end", "2022-04-16T13:17:00Z"},
                             {"vatcan_code", "abc"},
                             {"participants",
                              nlohmann::json::array(
                                      {{
                                               {"cid", 1203533},
                                               {"origin", "EGKK"},
                                               {"destination", 123},
                                       },
                                       {
                                               {"cid", 1203534},
                                               {"origin", "EGKK"},
                                               {"destination", "EGLL"},
                                       }}
                              )}}}

            ),
            [](const ::testing::TestParamInfo<EventDataParserBadDataTest::ParamType>& info) {
                return info.param.name;
            }
    );

    TEST_P(EventDataParserBadDataTest, ItRejectsBadData)
    {
        const nlohmann::json validEvent = {
                {"id", 1},
                {"flight_information_region_id", 1},
                {"name", "London Event"},
                {"date_start", "2022-04-16T13:16:00Z"},
                {"date_end", "2022-04-16T13:17:00Z"},
                {"vatcan_code", "abc"},
                {"participants",
                 nlohmann::json::array(
                         {{
                                  {"cid", 1203533},
                                  {"origin", "EGKK"},
                                  {"destination", "EGLL"},
                          },
                          {
                                  {"cid", 1203534},
                                  {"origin", "EGKK"},
                                  {"destination", "EGLL"},
                          }}
                 )}};

        auto data = nlohmann::json::array({GetParam().data});
        data.push_back(validEvent);
        auto events = parser.ParseEvents(nlohmann::json{{"events", data}}, firs);

        EXPECT_EQ(1, events->Count());

        const auto event1 = events->Get(1);
        EXPECT_NE(nullptr, event1);
        EXPECT_EQ(1, event1->Id());
    }
}// namespace ECFMPTest::Api
