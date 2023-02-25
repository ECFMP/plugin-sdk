#include "api/FlightInformationRegionDataParser.h"
#include "flightinformationregion/ConcreteFlightInformationRegionCollection.h"
#include "flow-sdk/FlightInformationRegion.h"
#include "mock/MockLogger.h"
#include "nlohmann/json.hpp"
#include "nlohmann/json_fwd.hpp"

namespace FlowSdkTest::Api {
    class FlightInformationRegionDataParserTest : public testing::Test
    {
        public:
        FlightInformationRegionDataParserTest()
            : firs(std::make_shared<FlowSdk::FlightInformationRegion::ConcreteFlightInformationRegionCollection>()),
              mockLogger(std::make_shared<testing::NiceMock<Log::MockLogger>>()), parser(firs, mockLogger)
        {}

        std::shared_ptr<FlowSdk::FlightInformationRegion::ConcreteFlightInformationRegionCollection> firs;
        std::shared_ptr<testing::NiceMock<Log::MockLogger>> mockLogger;
        FlowSdk::Api::FlightInformationRegionDataParser parser;
    };

    TEST_F(FlightInformationRegionDataParserTest, ItDoesNothingIfDataNotArray)
    {
        parser.OnEvent(nlohmann::json::object());
        EXPECT_EQ(0, firs->Count());
    }

    TEST_F(FlightInformationRegionDataParserTest, ItParsesFirs)
    {
        parser.OnEvent(nlohmann::json::array(
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
        ));
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
            : firs(std::make_shared<FlowSdk::FlightInformationRegion::ConcreteFlightInformationRegionCollection>()),
              mockLogger(std::make_shared<testing::NiceMock<Log::MockLogger>>()), parser(firs, mockLogger)
        {}

        std::shared_ptr<FlowSdk::FlightInformationRegion::ConcreteFlightInformationRegionCollection> firs;
        std::shared_ptr<testing::NiceMock<Log::MockLogger>> mockLogger;
        FlowSdk::Api::FlightInformationRegionDataParser parser;
    };

    TEST_P(FlightInformationRegionDataParserBadDataTest, ItRejectsBadData)
    {
        const nlohmann::json validRegion = {
                {"id", 1},
                {"identifier", "EGTT"},
                {"name", "London"},
        };

        auto data = nlohmann::json::array({validRegion});
        data.push_back(GetParam().data);
        parser.OnEvent(data);

        EXPECT_EQ(1, firs->Count());

        const auto fir1 = firs->Get(1);
        EXPECT_NE(nullptr, fir1);
        EXPECT_EQ(1, fir1->Id());
        EXPECT_EQ("EGTT", fir1->Identifier());
        EXPECT_EQ("London", fir1->Name());
    }

    INSTANTIATE_TEST_SUITE_P(
            FlightInformationRegionDataParserBadDataTestCases, FlightInformationRegionDataParserBadDataTest,
            testing::Values(
                    BadFlightInformationRegionDataCheck{
                            "no_id",
                            {
                                    {"id", 1},
                                    {"identifier", "EGTT"},
                                    {"name", "London"},
                            }},
                    BadFlightInformationRegionDataCheck{
                            "id_not_integer",
                            {
                                    {"id", 1.5},
                                    {"identifier", "EGTT"},
                                    {"name", "London"},
                            }},
                    BadFlightInformationRegionDataCheck{
                            "id_not_number",
                            {
                                    {"id", "abc"},
                                    {"identifier", "EGTT"},
                                    {"name", "London"},
                            }},
                    BadFlightInformationRegionDataCheck{
                            "no_identifier",
                            {
                                    {"id", 1},
                                    {"name", "London"},
                            }},
                    BadFlightInformationRegionDataCheck{
                            "identifier_not_string",
                            {
                                    {"id", 1},
                                    {"identifier", 123},
                                    {"name", "London"},
                            }},
                    BadFlightInformationRegionDataCheck{
                            "no_name",
                            {
                                    {"id", 1},
                                    {"identifier", "EGTT"},
                            }},
                    BadFlightInformationRegionDataCheck{
                            "name_not_string",
                            {
                                    {"id", 1},
                                    {"identifier", "EGTT"},
                                    {"name", 123},
                            }}
            ),
            [](const ::testing::TestParamInfo<FlightInformationRegionDataParserBadDataTest::ParamType>& info) {
                return info.param.name;
            }
    );
}// namespace FlowSdkTest::Api
