#include "api/FlowMeasureMeasureParser.h"
#include "flow-sdk/Measure.h"
#include "mock/MockLogger.h"
#include "nlohmann/json.hpp"

namespace FlowSdkTest::Api {

    template<typename ValueType>
    struct FlowMeasureMeasureTestCase {
        std::string description;
        std::string jsonType;
        ValueType jsonValue;
        FlowSdk::FlowMeasure::MeasureType expectedType;
        ValueType expectedValue;
    };

    // Integer tests (MilesInTrail, MinimumDepartureInterval, AverageDepartureInterval, PerHour, MaxIndicatedAirspeed,
    // IndicatedAirspeedReduction)
    class FlowMeasureMeasureParserIntegerTest : public testing::TestWithParam<FlowMeasureMeasureTestCase<int>>
    {
        public:
        FlowMeasureMeasureParserIntegerTest()
            : mockLogger(std::make_shared<testing::NiceMock<Log::MockLogger>>()), parser(mockLogger)
        {}

        std::shared_ptr<testing::NiceMock<Log::MockLogger>> mockLogger;
        FlowSdk::Api::FlowMeasureMeasureParser parser;
    };

    TEST_P(FlowMeasureMeasureParserIntegerTest, ItParsesIntegerMeasures)
    {
        const auto& param = GetParam();
        nlohmann::json json;
        json["type"] = param.jsonType;
        json["value"] = param.jsonValue;
        auto measure = parser.Parse(json);
        ASSERT_NE(measure, nullptr);
        ASSERT_EQ(measure->Type(), param.expectedType);
        ASSERT_EQ(measure->IntegerValue(), param.expectedValue);
    }

    INSTANTIATE_TEST_SUITE_P(
            FlowMeasureMeasureParserIntegerTest, FlowMeasureMeasureParserIntegerTest,
            testing::Values(
                    FlowMeasureMeasureTestCase<int>{
                            "miles_in_trail", "miles_in_trail", 5, FlowSdk::FlowMeasure::MeasureType::MilesInTrail, 5},
                    FlowMeasureMeasureTestCase<int>{
                            "minimum_departure_interval", "minimum_departure_interval", 5,
                            FlowSdk::FlowMeasure::MeasureType::MinimumDepartureInterval, 5},
                    FlowMeasureMeasureTestCase<int>{
                            "per_hour", "per_hour", 5, FlowSdk::FlowMeasure::MeasureType::PerHour, 5},
                    FlowMeasureMeasureTestCase<int>{
                            "max_indicated_airspeed", "max_ias", 5,
                            FlowSdk::FlowMeasure::MeasureType::MaxIndicatedAirspeed, 5},
                    FlowMeasureMeasureTestCase<int>{
                            "indicated_airspeed_reduction", "ias_reduction", 5,
                            FlowSdk::FlowMeasure::MeasureType::IndicatedAirspeedReduction, 5}
            ),
            [](const testing::TestParamInfo<FlowMeasureMeasureTestCase<int>>& info) {
                return info.param.description;
            }
    );

    // Double tests (max_mach, mach_reduction)
    class FlowMeasureMeasureParserDoubleTest : public testing::TestWithParam<FlowMeasureMeasureTestCase<double>>
    {
        public:
        FlowMeasureMeasureParserDoubleTest()
            : mockLogger(std::make_shared<testing::NiceMock<Log::MockLogger>>()), parser(mockLogger)
        {}

        std::shared_ptr<testing::NiceMock<Log::MockLogger>> mockLogger;
        FlowSdk::Api::FlowMeasureMeasureParser parser;
    };

    TEST_P(FlowMeasureMeasureParserDoubleTest, ItParsesDoubleMeasures)
    {
        const auto& param = GetParam();
        nlohmann::json json;
        json["type"] = param.jsonType;
        json["value"] = param.jsonValue;
        auto measure = parser.Parse(json);
        ASSERT_NE(measure, nullptr);
        ASSERT_EQ(measure->Type(), param.expectedType);
        ASSERT_DOUBLE_EQ(measure->DoubleValue(), param.expectedValue);
    }

    INSTANTIATE_TEST_SUITE_P(
            FlowMeasureMeasureParserDoubleTest, FlowMeasureMeasureParserDoubleTest,
            testing::Values(
                    FlowMeasureMeasureTestCase<double>{
                            "max_mach", "max_mach", 5.0, FlowSdk::FlowMeasure::MeasureType::MaxMach, 5.0},
                    FlowMeasureMeasureTestCase<double>{
                            "mach_reduction", "mach_reduction", 5.0, FlowSdk::FlowMeasure::MeasureType::MachReduction,
                            5.0}
            ),
            [](const testing::TestParamInfo<FlowMeasureMeasureTestCase<double>>& info) {
                return info.param.description;
            }
    );

    // Set tests (mandatory_route)
    class FlowMeasureMeasureParserSetTest
        : public testing::TestWithParam<FlowMeasureMeasureTestCase<std::set<std::string>>>
    {
        public:
        FlowMeasureMeasureParserSetTest()
            : mockLogger(std::make_shared<testing::NiceMock<Log::MockLogger>>()), parser(mockLogger)
        {}

        std::shared_ptr<testing::NiceMock<Log::MockLogger>> mockLogger;
        FlowSdk::Api::FlowMeasureMeasureParser parser;
    };

    TEST_P(FlowMeasureMeasureParserSetTest, ItParsesSetMeasures)
    {
        const auto& param = GetParam();
        nlohmann::json json;
        json["type"] = param.jsonType;
        json["value"] = param.jsonValue;
        auto measure = parser.Parse(json);
        ASSERT_NE(measure, nullptr);
        ASSERT_EQ(measure->Type(), param.expectedType);
        ASSERT_EQ(measure->SetValue(), param.expectedValue);
    }

    INSTANTIATE_TEST_SUITE_P(
            FlowMeasureMeasureParserSetTest, FlowMeasureMeasureParserSetTest,
            testing::Values(FlowMeasureMeasureTestCase<std::set<std::string>>{
                    "mandatory_route",
                    "mandatory_route",
                    {"A", "B", "C"},
                    FlowSdk::FlowMeasure::MeasureType::MandatoryRoute,
                    {"A", "B", "C"}}),
            [](const testing::TestParamInfo<FlowMeasureMeasureTestCase<std::set<std::string>>>& info) {
                return info.param.description;
            }
    );

    // No value test (ground_stop and prohibit)
    class FlowMeasureParserNoValueTest
        : public testing::TestWithParam<FlowMeasureMeasureTestCase<nlohmann::json::value_t>>
    {
        public:
        FlowMeasureParserNoValueTest()
            : mockLogger(std::make_shared<testing::NiceMock<Log::MockLogger>>()), parser(mockLogger)
        {}

        std::shared_ptr<testing::NiceMock<Log::MockLogger>> mockLogger;
        FlowSdk::Api::FlowMeasureMeasureParser parser;
    };

    TEST_P(FlowMeasureParserNoValueTest, ItParsesNoValueMeasures)
    {
        const auto& param = GetParam();
        nlohmann::json json;
        json["type"] = param.jsonType;
        json["value"] = param.jsonValue;
        auto measure = parser.Parse(json);
        ASSERT_NE(measure, nullptr);
        ASSERT_EQ(measure->Type(), param.expectedType);
    }

    INSTANTIATE_TEST_SUITE_P(
            FlowMeasureParserNoValueTest, FlowMeasureParserNoValueTest,
            testing::Values(
                    FlowMeasureMeasureTestCase<nlohmann::json::value_t>{
                            "ground_stop", "ground_stop", nlohmann::json::value_t::null,
                            FlowSdk::FlowMeasure::MeasureType::GroundStop, nlohmann::json::value_t::null},
                    FlowMeasureMeasureTestCase<nlohmann::json::value_t>{
                            "prohibit", "prohibit", nlohmann::json::value_t::null,
                            FlowSdk::FlowMeasure::MeasureType::Prohibit, nlohmann::json::value_t::null}
            ),
            [](const testing::TestParamInfo<FlowMeasureMeasureTestCase<nlohmann::json::value_t>>& info) {
                return info.param.description;
            }
    );

    struct FlowMeasureBadDataTestCase {
        std::string description;
        nlohmann::json json;
    };

    class FlowMeasureMeasureParserBadDataTest : public testing::TestWithParam<FlowMeasureBadDataTestCase>
    {
        public:
        FlowMeasureMeasureParserBadDataTest()
            : mockLogger(std::make_shared<testing::NiceMock<Log::MockLogger>>()), parser(mockLogger)
        {}

        std::shared_ptr<testing::NiceMock<Log::MockLogger>> mockLogger;
        FlowSdk::Api::FlowMeasureMeasureParser parser;
    };

    TEST_P(FlowMeasureMeasureParserBadDataTest, ItReturnsNullptrBadData)
    {
        const auto& param = GetParam();
        ASSERT_EQ(nullptr, parser.Parse(param.json));
    }

    INSTANTIATE_TEST_SUITE_P(
            FlowMeasureMeasureParserBadDataTest, FlowMeasureMeasureParserBadDataTest,
            testing::Values(
                    FlowMeasureBadDataTestCase{"not_an_object", nlohmann::json::array()},
                    FlowMeasureBadDataTestCase{"no_type_field", {{"value", 5}}},
                    FlowMeasureBadDataTestCase{"type_field_is_not_string", {{"type", 5}, {"value", 5}}},
                    FlowMeasureBadDataTestCase{"no_value_field", {{"type", "miles_in_trail"}}},
                    FlowMeasureBadDataTestCase{"type_field_is_not_a_known_type", {{"type", "foo"}, {"value", 5}}},
                    FlowMeasureBadDataTestCase{
                            "miles_in_trail_type_with_non_integer_value",
                            {{"type", "miles_in_trail"}, {"value", "foo"}}},
                    FlowMeasureBadDataTestCase{
                            "minimum_departure_interval_type_with_non_integer_value",
                            {{"type", "minimum_departure_interval"}, {"value", "foo"}}},
                    FlowMeasureBadDataTestCase{
                            "average_departure_interval_type_with_non_integer_value",
                            {{"type", "average_departure_interval"}, {"value", "foo"}}},
                    FlowMeasureBadDataTestCase{
                            "max_ias_type_with_non_integer_value",
                            {{"type", "max_ias"}, {"value", "foo"}}},
                    FlowMeasureBadDataTestCase{
                            "ias_reduction_type_with_non_integer_value",
                            {{"type", "ias_reduction"}, {"value", "foo"}}},
                    FlowMeasureBadDataTestCase{
                            "per_hour_type_with_non_integer_value",
                            {{"type", "per_hour"}, {"value", "foo"}}},
                    FlowMeasureBadDataTestCase{
                            "max_mach_type_with_non_double_value",
                            {{"type", "max_mach"}, {"value", "foo"}}},
                    FlowMeasureBadDataTestCase{
                            "mach_reduction_type_with_non_double_value",
                            {{"type", "mach_reduction"}, {"value", "foo"}}},
                    FlowMeasureBadDataTestCase{
                            "mandatory_route_type_with_non_array_value",
                            {{"type", "mandatory_route"}, {"value", 5}}},
                    FlowMeasureBadDataTestCase{
                            "mandatory_route_type_with_non_string_array_value",
                            {{"type", "mandatory_route"}, {"value", {5, 6, 7}}}}
            ),
            [](const testing::TestParamInfo<FlowMeasureBadDataTestCase>& info) {
                return info.param.description;
            }
    );
}// namespace FlowSdkTest::Api
