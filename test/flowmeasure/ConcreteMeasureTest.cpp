#include "flowmeasure/ConcreteMeasure.h"
#include "flowmeasure/ConcreteMeasureFactory.h"

namespace FlowSdkTest::FlowMeasure {

    template<typename MeasureValueType>
    struct MeasureTestCase {
        // The type of measure
        FlowSdk::FlowMeasure::MeasureType measureType;

        // The value of the measure
        MeasureValueType value;
    };

    class ConcreteMeasureNoValueTest : public testing::TestWithParam<MeasureTestCase<int>>
    {
        public:
        [[nodiscard]] static auto MakeMeasure(FlowSdk::FlowMeasure::MeasureType type)
                -> FlowSdk::FlowMeasure::ConcreteMeasure
        {
            switch (type) {
            case FlowSdk::FlowMeasure::MeasureType::GroundStop:
                return FlowSdk::FlowMeasure::GroundStop();
            case FlowSdk::FlowMeasure::MeasureType::Prohibit:
                return FlowSdk::FlowMeasure::Prohibit();
            default:
                throw std::invalid_argument("Bad measure type");
            }
        }
    };

    TEST_P(ConcreteMeasureNoValueTest, ItHasVoidValuedMeasures)
    {
        const auto measure = MakeMeasure(GetParam().measureType);

        EXPECT_EQ(GetParam().measureType, measure.Type());
        EXPECT_THROW(static_cast<void>(measure.IntegerValue()), FlowSdk::FlowMeasure::IllegalFlowMeasureValueException);
        EXPECT_THROW(static_cast<void>(measure.DoubleValue()), FlowSdk::FlowMeasure::IllegalFlowMeasureValueException);
        EXPECT_THROW(static_cast<void>(measure.SetValue()), FlowSdk::FlowMeasure::IllegalFlowMeasureValueException);
    }

    INSTANTIATE_TEST_SUITE_P(ConcreteMeasureNoValueTestCases, ConcreteMeasureNoValueTest,
                             testing::Values(MeasureTestCase<int>{FlowSdk::FlowMeasure::MeasureType::Prohibit, 5},
                                             MeasureTestCase<int>{FlowSdk::FlowMeasure::MeasureType::GroundStop, 6}),
                             [](const ::testing::TestParamInfo<ConcreteMeasureNoValueTest::ParamType>& info) {
                                 return "type_" + std::to_string((int) info.param.measureType) + "_no_value";
                             });

    class ConcreteMeasureIntegerValueTest : public testing::TestWithParam<MeasureTestCase<int>>
    {
        public:
        [[nodiscard]] static auto MakeMeasure(FlowSdk::FlowMeasure::MeasureType type, int value)
                -> FlowSdk::FlowMeasure::ConcreteMeasure
        {
            switch (type) {
            case FlowSdk::FlowMeasure::MeasureType::MinimumDepartureInterval:
                return FlowSdk::FlowMeasure::MinimumDepartureInterval(value);
            case FlowSdk::FlowMeasure::MeasureType::AverageDepartureInterval:
                return FlowSdk::FlowMeasure::AverageDepartureInterval(value);
            case FlowSdk::FlowMeasure::MeasureType::PerHour:
                return FlowSdk::FlowMeasure::PerHour(value);
            case FlowSdk::FlowMeasure::MeasureType::MilesInTrail:
                return FlowSdk::FlowMeasure::MilesInTrail(value);
            case FlowSdk::FlowMeasure::MeasureType::MaxIndicatedAirspeed:
                return FlowSdk::FlowMeasure::MaxIndicatedAirspeed(value);
            case FlowSdk::FlowMeasure::MeasureType::IndicatedAirspeedReduction:
                return FlowSdk::FlowMeasure::IndicatedAirspeedReduction(value);
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
        EXPECT_THROW(static_cast<void>(measure.DoubleValue()), FlowSdk::FlowMeasure::IllegalFlowMeasureValueException);
        EXPECT_THROW(static_cast<void>(measure.SetValue()), FlowSdk::FlowMeasure::IllegalFlowMeasureValueException);
    }

    INSTANTIATE_TEST_SUITE_P(
            ConcreteMeasureIntegerValueTestCases, ConcreteMeasureIntegerValueTest,
            testing::Values(MeasureTestCase<int>{FlowSdk::FlowMeasure::MeasureType::MinimumDepartureInterval, 5},
                            MeasureTestCase<int>{FlowSdk::FlowMeasure::MeasureType::AverageDepartureInterval, 6},
                            MeasureTestCase<int>{FlowSdk::FlowMeasure::MeasureType::PerHour, 7},
                            MeasureTestCase<int>{FlowSdk::FlowMeasure::MeasureType::MilesInTrail, 8},
                            MeasureTestCase<int>{FlowSdk::FlowMeasure::MeasureType::MaxIndicatedAirspeed, 9},
                            MeasureTestCase<int>{FlowSdk::FlowMeasure::MeasureType::IndicatedAirspeedReduction, 10}),
            [](const ::testing::TestParamInfo<ConcreteMeasureIntegerValueTest::ParamType>& info) {
                return "type_" + std::to_string((int) info.param.measureType) + "_value_"
                        + std::to_string(info.param.value);
            });

    class ConcreteMeasureDoubleValueTest : public testing::TestWithParam<MeasureTestCase<double>>
    {
        public:
        [[nodiscard]] static auto MakeMeasure(FlowSdk::FlowMeasure::MeasureType type, double value)
                -> FlowSdk::FlowMeasure::ConcreteMeasure
        {
            switch (type) {
            case FlowSdk::FlowMeasure::MeasureType::MaxMach:
                return FlowSdk::FlowMeasure::MaxMach(value);
            case FlowSdk::FlowMeasure::MeasureType::MachReduction:
                return FlowSdk::FlowMeasure::MachReduction(value);
            default:
                throw std::invalid_argument("Bad measure type");
            }
        }
    };

    TEST_P(ConcreteMeasureDoubleValueTest, ItHasDoubleValuedMeasures)
    {
        FlowSdk::FlowMeasure::ConcreteMeasure measure(GetParam().measureType, GetParam().value);

        EXPECT_EQ(GetParam().measureType, measure.Type());
        EXPECT_DOUBLE_EQ(GetParam().value, measure.DoubleValue());
        EXPECT_THROW(static_cast<void>(measure.IntegerValue()), FlowSdk::FlowMeasure::IllegalFlowMeasureValueException);
        EXPECT_THROW(static_cast<void>(measure.SetValue()), FlowSdk::FlowMeasure::IllegalFlowMeasureValueException);
    }

    INSTANTIATE_TEST_SUITE_P(ConcreteMeasureDoubleValueTestCases, ConcreteMeasureDoubleValueTest,
                             testing::Values(MeasureTestCase<double>{FlowSdk::FlowMeasure::MeasureType::MaxMach, 5.0},
                                             MeasureTestCase<double>{FlowSdk::FlowMeasure::MeasureType::MachReduction,
                                                                     6.0}),
                             [](const ::testing::TestParamInfo<ConcreteMeasureDoubleValueTest::ParamType>& info) {
                                 return "type_" + std::to_string((int) info.param.measureType) + "_value_"
                                         + std::to_string((int) info.param.value);
                             });

    class ConcreteMeasureSetValueTest : public testing::TestWithParam<MeasureTestCase<std::set<std::string>>>
    {
        public:
        [[nodiscard]] static auto MakeMeasure(FlowSdk::FlowMeasure::MeasureType type, std::set<std::string> value)
                -> FlowSdk::FlowMeasure::ConcreteMeasure
        {
            switch (type) {
            case FlowSdk::FlowMeasure::MeasureType::MandatoryRoute:
                return FlowSdk::FlowMeasure::MandatoryRoute(value);
            default:
                throw std::invalid_argument("Bad measure type");
            }
        }
    };

    TEST_P(ConcreteMeasureSetValueTest, ItHasMandatoryRouteValuedMeasures)
    {
        FlowSdk::FlowMeasure::ConcreteMeasure measure(GetParam().measureType, GetParam().value);

        EXPECT_EQ(GetParam().measureType, measure.Type());
        EXPECT_EQ(GetParam().value, measure.SetValue());
        EXPECT_THROW(static_cast<void>(measure.DoubleValue()), FlowSdk::FlowMeasure::IllegalFlowMeasureValueException);
        EXPECT_THROW(static_cast<void>(measure.IntegerValue()), FlowSdk::FlowMeasure::IllegalFlowMeasureValueException);
    }

    INSTANTIATE_TEST_SUITE_P(ConcreteMeasureSetValueTestCases, ConcreteMeasureSetValueTest,
                             testing::Values(MeasureTestCase<std::set<std::string>>{
                                     FlowSdk::FlowMeasure::MeasureType::MandatoryRoute,
                                     std::set<std::string>({"foo", "bar"})}),
                             [](const ::testing::TestParamInfo<ConcreteMeasureSetValueTest::ParamType>& info) {
                                 return "type_" + std::to_string((int) info.param.measureType);
                             });
}// namespace FlowSdkTest::FlowMeasure
