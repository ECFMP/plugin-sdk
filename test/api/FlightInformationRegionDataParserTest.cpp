#include "api/FlightInformationRegionDataParser.h"
#include "api/ConcreteStringIdentifiedApiElementCollection.h"
#include "api/InternalElementCollectionTypes.h"
#include "ECFMP/FlightInformationRegion.h"
#include "mock/MockLogger.h"
#include "nlohmann/json.hpp"

namespace ECFMPTest::Api {
    class FlightInformationRegionDataParserTest : public testing::Test
    {
        public:
        FlightInformationRegionDataParserTest()
            : firs(std::make_shared<ECFMP::Api::ConcreteStringIdentifiedApiElementCollection<
                           ECFMP::FlightInformationRegion::FlightInformationRegion>>()),
              mockLogger(std::make_shared<testing::NiceMock<Log::MockLogger>>()), parser(firs, mockLogger)
        {}

        std::shared_ptr<ECFMP::Api::InternalFlightInformationRegionCollection> firs;
        std::shared_ptr<testing::NiceMock<Log::MockLogger>> mockLogger;
        ECFMP::Api::FlightInformationRegionDataParser parser;
    };

    TEST_F(FlightInformationRegionDataParserTest, ItDoesNothingIfDataNotObject)
    {
        parser.OnEvent(nlohmann::json::array());
        EXPECT_EQ(0, firs->Count());
    }

    TEST_F(FlightInformationRegionDataParserTest, ItDoesNothingIfDataDoesNotContainFirs)
    {
        parser.OnEvent(nlohmann::json{{"not_flight_information_regions", "abc"}});
        EXPECT_EQ(0, firs->Count());
    }

    TEST_F(FlightInformationRegionDataParserTest, ItDoesNothingIfFirsNotArray)
    {
        parser.OnEvent(nlohmann::json{{"flight_information_regions", "abc"}});
        EXPECT_EQ(0, firs->Count());
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

        parser.OnEvent(nlohmann::json{{"flight_information_regions", firData}});
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
            : firs(std::make_shared<ECFMP::Api::ConcreteStringIdentifiedApiElementCollection<
                           ECFMP::FlightInformationRegion::FlightInformationRegion>>()),
              mockLogger(std::make_shared<testing::NiceMock<Log::MockLogger>>()), parser(firs, mockLogger)
        {}

        std::shared_ptr<ECFMP::Api::ConcreteStringIdentifiedApiElementCollection<
                ECFMP::FlightInformationRegion::FlightInformationRegion>>
                firs;
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
        parser.OnEvent(nlohmann::json{{"flight_information_regions", data}});

        EXPECT_EQ(1, firs->Count());

        const auto fir1 = firs->Get(1);
        EXPECT_NE(nullptr, fir1);
        EXPECT_EQ(1, fir1->Id());
        EXPECT_EQ("EGTT", fir1->Identifier());
        EXPECT_EQ("London", fir1->Name());
    }
}// namespace ECFMPTest::Api
