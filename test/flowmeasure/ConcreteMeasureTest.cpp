#include "flowmeasure/ConcreteMeasure.h"
#include "ECFMP/flowmeasure/FlowMeasure.h"
#include "flowmeasure/ConcreteMeasureFactory.h"

namespace ECFMPTest::FlowMeasure {

    template<typename MeasureValueType>
    struct MeasureTestCase {
        // The type of measure
        ECFMP::FlowMeasure::MeasureType measureType;

        // The value of the measure
        MeasureValueType value;
    };

    class ConcreteMeasureNoValueTest : public testing::TestWithParam<MeasureTestCase<int>>
    {
        public:
        [[nodiscard]] static auto MakeMeasure(ECFMP::FlowMeasure::MeasureType type)
                -> ECFMP::FlowMeasure::ConcreteMeasure
        {
            switch (type) {
            case ECFMP::FlowMeasure::MeasureType::GroundStop:
                return ECFMP::FlowMeasure::GroundStop();
            case ECFMP::FlowMeasure::MeasureType::Prohibit:
                return ECFMP::FlowMeasure::Prohibit();
            default:
                throw std::invalid_argument("Bad measure type");
            }
        }
    };

    TEST_P(ConcreteMeasureNoValueTest, ItHasVoidValuedMeasures)
    {
        const auto measure = MakeMeasure(GetParam().measureType);

        EXPECT_EQ(GetParam().measureType, measure.Type());
        EXPECT_THROW(static_cast<void>(measure.IntegerValue()), ECFMP::FlowMeasure::IllegalFlowMeasureValueException);
        EXPECT_THROW(static_cast<void>(measure.DoubleValue()), ECFMP::FlowMeasure::IllegalFlowMeasureValueException);
        EXPECT_THROW(static_cast<void>(measure.SetValue()), ECFMP::FlowMeasure::IllegalFlowMeasureValueException);
    }

    INSTANTIATE_TEST_SUITE_P(
            ConcreteMeasureNoValueTestCases, ConcreteMeasureNoValueTest,
            testing::Values(
                    MeasureTestCase<int>{ECFMP::FlowMeasure::MeasureType::Prohibit, 5},
                    MeasureTestCase<int>{ECFMP::FlowMeasure::MeasureType::GroundStop, 6}
            ),
            [](const ::testing::TestParamInfo<ConcreteMeasureNoValueTest::ParamType>& info) {
                return "type_" + std::to_string((int) info.param.measureType) + "_no_value";
            }
    );

    class ConcreteMeasureIntegerValueTest : public testing::TestWithParam<MeasureTestCase<int>>
    {
        public:
        [[nodiscard]] static auto MakeMeasure(ECFMP::FlowMeasure::MeasureType type, int value)
                -> ECFMP::FlowMeasure::ConcreteMeasure
        {
            switch (type) {
            case ECFMP::FlowMeasure::MeasureType::MinimumDepartureInterval:
                return ECFMP::FlowMeasure::MinimumDepartureInterval(value);
            case ECFMP::FlowMeasure::MeasureType::AverageDepartureInterval:
                return ECFMP::FlowMeasure::AverageDepartureInterval(value);
            case ECFMP::FlowMeasure::MeasureType::PerHour:
                return ECFMP::FlowMeasure::PerHour(value);
            case ECFMP::FlowMeasure::MeasureType::MilesInTrail:
                return ECFMP::FlowMeasure::MilesInTrail(value);
            case ECFMP::FlowMeasure::MeasureType::MaxIndicatedAirspeed:
                return ECFMP::FlowMeasure::MaxIndicatedAirspeed(value);
            case ECFMP::FlowMeasure::MeasureType::IndicatedAirspeedReduction:
                return ECFMP::FlowMeasure::IndicatedAirspeedReduction(value);
            default:
                throw std::invalid_argument("Bad measure type");
            }
        }
    };

    TEST_P(ConcreteMeasureIntegerValueTest, ItHasIntegerValuedMeasures)
    {
        const auto measure = MakeMeasure(GetParam().measureType, GetParam().value);

        EXPECT_EQ(GetParam().measureType, measure.Type());
        EXPECT_EQ(GetParam().value, measure.IntegerValue());
        EXPECT_THROW(static_cast<void>(measure.DoubleValue()), ECFMP::FlowMeasure::IllegalFlowMeasureValueException);
        EXPECT_THROW(static_cast<void>(measure.SetValue()), ECFMP::FlowMeasure::IllegalFlowMeasureValueException);
    }

    INSTANTIATE_TEST_SUITE_P(
            ConcreteMeasureIntegerValueTestCases, ConcreteMeasureIntegerValueTest,
            testing::Values(
                    MeasureTestCase<int>{ECFMP::FlowMeasure::MeasureType::MinimumDepartureInterval, 5},
                    MeasureTestCase<int>{ECFMP::FlowMeasure::MeasureType::AverageDepartureInterval, 6},
                    MeasureTestCase<int>{ECFMP::FlowMeasure::MeasureType::PerHour, 7},
                    MeasureTestCase<int>{ECFMP::FlowMeasure::MeasureType::MilesInTrail, 8},
                    MeasureTestCase<int>{ECFMP::FlowMeasure::MeasureType::MaxIndicatedAirspeed, 9},
                    MeasureTestCase<int>{ECFMP::FlowMeasure::MeasureType::IndicatedAirspeedReduction, 10}
            ),
            [](const ::testing::TestParamInfo<ConcreteMeasureIntegerValueTest::ParamType>& info) {
                return "type_" + std::to_string((int) info.param.measureType) + "_value_"
                        + std::to_string(info.param.value);
            }
    );

    class ConcreteMeasureDoubleValueTest : public testing::TestWithParam<MeasureTestCase<double>>
    {
        public:
        [[nodiscard]] static auto MakeMeasure(ECFMP::FlowMeasure::MeasureType type, double value)
                -> ECFMP::FlowMeasure::ConcreteMeasure
        {
            switch (type) {
            case ECFMP::FlowMeasure::MeasureType::MaxMach:
                return ECFMP::FlowMeasure::MaxMach(value);
            case ECFMP::FlowMeasure::MeasureType::MachReduction:
                return ECFMP::FlowMeasure::MachReduction(value);
            default:
                throw std::invalid_argument("Bad measure type");
            }
        }
    };

    TEST_P(ConcreteMeasureDoubleValueTest, ItHasDoubleValuedMeasures)
    {
        ECFMP::FlowMeasure::ConcreteMeasure measure(GetParam().measureType, GetParam().value);

        EXPECT_EQ(GetParam().measureType, measure.Type());
        EXPECT_DOUBLE_EQ(GetParam().value, measure.DoubleValue());
        EXPECT_THROW(static_cast<void>(measure.IntegerValue()), ECFMP::FlowMeasure::IllegalFlowMeasureValueException);
        EXPECT_THROW(static_cast<void>(measure.SetValue()), ECFMP::FlowMeasure::IllegalFlowMeasureValueException);
    }

    INSTANTIATE_TEST_SUITE_P(
            ConcreteMeasureDoubleValueTestCases, ConcreteMeasureDoubleValueTest,
            testing::Values(
                    MeasureTestCase<double>{ECFMP::FlowMeasure::MeasureType::MaxMach, 5.0},
                    MeasureTestCase<double>{ECFMP::FlowMeasure::MeasureType::MachReduction, 6.0}
            ),
            [](const ::testing::TestParamInfo<ConcreteMeasureDoubleValueTest::ParamType>& info) {
                return "type_" + std::to_string((int) info.param.measureType) + "_value_"
                        + std::to_string((int) info.param.value);
            }
    );

    class ConcreteMeasureSetValueTest : public testing::TestWithParam<MeasureTestCase<std::set<std::string>>>
    {
        public:
        [[nodiscard]] static auto MakeMeasure(ECFMP::FlowMeasure::MeasureType type, std::set<std::string> value)
                -> ECFMP::FlowMeasure::ConcreteMeasure
        {
            switch (type) {
            case ECFMP::FlowMeasure::MeasureType::MandatoryRoute:
                return ECFMP::FlowMeasure::MandatoryRoute(value);
            default:
                throw std::invalid_argument("Bad measure type");
            }
        }
    };

    TEST_P(ConcreteMeasureSetValueTest, ItHasMandatoryRouteValuedMeasures)
    {
        ECFMP::FlowMeasure::ConcreteMeasure measure(GetParam().measureType, GetParam().value);

        EXPECT_EQ(GetParam().measureType, measure.Type());
        EXPECT_EQ(GetParam().value, measure.SetValue());
        EXPECT_THROW(static_cast<void>(measure.DoubleValue()), ECFMP::FlowMeasure::IllegalFlowMeasureValueException);
        EXPECT_THROW(static_cast<void>(measure.IntegerValue()), ECFMP::FlowMeasure::IllegalFlowMeasureValueException);
    }

    INSTANTIATE_TEST_SUITE_P(
            ConcreteMeasureSetValueTestCases, ConcreteMeasureSetValueTest,
            testing::Values(MeasureTestCase<std::set<std::string>>{
                    ECFMP::FlowMeasure::MeasureType::MandatoryRoute, std::set<std::string>({"foo", "bar"})}),
            [](const ::testing::TestParamInfo<ConcreteMeasureSetValueTest::ParamType>& info) {
                return "type_" + std::to_string((int) info.param.measureType);
            }
    );

    struct ConcreteMeasureDescriptionTestCase {
        // Test description
        std::string testDescription;

        // The type of measure
        std::function<ECFMP::FlowMeasure::ConcreteMeasure()> measureGenerator;

        // The expected description
        std::string expectedDescription;
    };

    class ConcreteMeasureDescriptionTest : public testing::TestWithParam<ConcreteMeasureDescriptionTestCase>
    {
        public:
    };

    TEST_P(ConcreteMeasureDescriptionTest, ItHasAnAppropriateDescription)
    {
        auto measure = GetParam().measureGenerator();
        EXPECT_EQ(GetParam().expectedDescription, measure.MeasureDescription());
    }

    INSTANTIATE_TEST_SUITE_P(
            ConcreteMeasureDescriptionTestCases, ConcreteMeasureDescriptionTest,
            testing::Values(
                    ConcreteMeasureDescriptionTestCase{
                            "minimum_departure_interval_seconds_only",
                            []() {
                                return ECFMP::FlowMeasure::MinimumDepartureInterval(5);
                            },
                            "Minimum Departure Interval: 5 seconds"},
                    ConcreteMeasureDescriptionTestCase{
                            "minimum_departure_interval_minutes_only",
                            []() {
                                return ECFMP::FlowMeasure::MinimumDepartureInterval(300);
                            },
                            "Minimum Departure Interval: 5 minutes"},
                    ConcreteMeasureDescriptionTestCase{
                            "minimum_departure_interval_minutes_and_seconds",
                            []() {
                                return ECFMP::FlowMeasure::MinimumDepartureInterval(305);
                            },
                            "Minimum Departure Interval: 5 minutes 5 seconds"},
                    ConcreteMeasureDescriptionTestCase{
                            "average_departure_interval_seconds_only",
                            []() {
                                return ECFMP::FlowMeasure::AverageDepartureInterval(5);
                            },
                            "Average Departure Interval: 5 seconds"},
                    ConcreteMeasureDescriptionTestCase{
                            "average_departure_interval_minutes_only",
                            []() {
                                return ECFMP::FlowMeasure::AverageDepartureInterval(300);
                            },
                            "Average Departure Interval: 5 minutes"},
                    ConcreteMeasureDescriptionTestCase{
                            "average_departure_interval_minutes_and_seconds",
                            []() {
                                return ECFMP::FlowMeasure::AverageDepartureInterval(305);
                            },
                            "Average Departure Interval: 5 minutes 5 seconds"},
                    ConcreteMeasureDescriptionTestCase{
                            "per_hour",
                            []() {
                                return ECFMP::FlowMeasure::PerHour(5);
                            },
                            "Per Hour: 5"},
                    ConcreteMeasureDescriptionTestCase{
                            "miles_in_trail",
                            []() {
                                return ECFMP::FlowMeasure::MilesInTrail(5);
                            },
                            "Miles in Trail: 5"},
                    // Max ias
                    ConcreteMeasureDescriptionTestCase{
                            "max_ias",
                            []() {
                                return ECFMP::FlowMeasure::MaxIndicatedAirspeed(5);
                            },
                            "Max IAS: 5kts"},
                    ConcreteMeasureDescriptionTestCase{
                            "ias_reduction",
                            []() {
                                return ECFMP::FlowMeasure::IndicatedAirspeedReduction(5);
                            },
                            "IAS Reduction: 5kts"},
                    ConcreteMeasureDescriptionTestCase{
                            "max_mach",
                            []() {
                                return ECFMP::FlowMeasure::MaxMach(0.89123);
                            },
                            "Max Mach: 0.89"},
                    ConcreteMeasureDescriptionTestCase{
                            "mach_reduction",
                            []() {
                                return ECFMP::FlowMeasure::MachReduction(0.89123);
                            },
                            "Mach Reduction: 0.89"},
                    ConcreteMeasureDescriptionTestCase{
                            "mandatory_route",
                            []() {
                                return ECFMP::FlowMeasure::MandatoryRoute(std::set<std::string>({"foo boo", "bar"}));
                            },
                            "Mandatory Route(s): bar, foo boo"},
                    ConcreteMeasureDescriptionTestCase{
                            "mandatory_route_empty",
                            []() {
                                return ECFMP::FlowMeasure::MandatoryRoute(std::set<std::string>());
                            },
                            "Mandatory Route(s): "},
                    ConcreteMeasureDescriptionTestCase{
                            "mandatory_route_single",
                            []() {
                                return ECFMP::FlowMeasure::MandatoryRoute(std::set<std::string>({"foo boo"}));
                            },
                            "Mandatory Route(s): foo boo"},
                    ConcreteMeasureDescriptionTestCase{
                            "mandatory_route_single_empty",
                            []() {
                                return ECFMP::FlowMeasure::MandatoryRoute(std::set<std::string>({" "}));
                            },
                            "Mandatory Route(s):  "},
                    ConcreteMeasureDescriptionTestCase{
                            "prohibit",
                            []() {
                                return ECFMP::FlowMeasure::Prohibit();
                            },
                            "Prohibit"},
                    ConcreteMeasureDescriptionTestCase{
                            "ground_stop",
                            []() {
                                return ECFMP::FlowMeasure::GroundStop();
                            },
                            "Ground Stop"}
            ),
            [](const ::testing::TestParamInfo<ConcreteMeasureDescriptionTest::ParamType>& info) {
                return info.param.testDescription;
            }
    );
}// namespace ECFMPTest::FlowMeasure
