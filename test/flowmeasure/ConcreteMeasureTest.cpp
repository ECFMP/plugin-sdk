#include "flowmeasure/ConcreteMeasure.h"
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
}// namespace ECFMPTest::FlowMeasure
