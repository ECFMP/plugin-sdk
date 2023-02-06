#include "flowmeasure/ConcreteLevelFilter.h"

namespace FlowSdkTest::FlowMeasure {

    enum ApplicabilityCheckType
    {
        Altitude = 0,
        Level = 1,
    };

    using LevelFilterApplicabilityCheck = struct LevelFilterApplicabilityCheck {
        // The type of check
        ApplicabilityCheckType checkType;

        // The filter type
        FlowSdk::FlowMeasure::LevelFilterType filterType;

        // The level for the filter
        int filterLevel;

        // The level to check against
        int checkLevel;

        // Expected applicability
        bool expectedApplicability;
    };

    class ConcreteLevelFilterTest : public testing::Test
    {
        public:
        ConcreteLevelFilterTest() : levelFilter(FlowSdk::FlowMeasure::LevelFilterType::At, 350)
        {}

        FlowSdk::FlowMeasure::ConcreteLevelFilter levelFilter;
    };

    TEST_F(ConcreteLevelFilterTest, ItReturnsLevel)
    {
        EXPECT_EQ(350, levelFilter.Level());
    }

    TEST_F(ConcreteLevelFilterTest, ItReturnsAltitude)
    {
        EXPECT_EQ(35000, levelFilter.Altitude());
    }

    TEST_F(ConcreteLevelFilterTest, ItReturnsType)
    {
        EXPECT_EQ(FlowSdk::FlowMeasure::LevelFilterType::At, levelFilter.Type());
    }

    class ConcreteLevelFilterApplicabilityTest : public testing::TestWithParam<LevelFilterApplicabilityCheck>
    {
    };

    TEST_P(ConcreteLevelFilterApplicabilityTest, ItChecksAirportApplicability)
    {
        FlowSdk::FlowMeasure::ConcreteLevelFilter filter(GetParam().filterType, GetParam().filterLevel);
        if (GetParam().checkType == ApplicabilityCheckType::Level) {
            EXPECT_EQ(GetParam().expectedApplicability, filter.ApplicableToLevel(GetParam().checkLevel));
        }
        else {
            EXPECT_EQ(GetParam().expectedApplicability, filter.ApplicableToAltitude(GetParam().checkLevel));
        }
    }

    INSTANTIATE_TEST_SUITE_P(
            ConcreteLevelFilterApplicabilityTestCases, ConcreteLevelFilterApplicabilityTest,
            testing::Values(LevelFilterApplicabilityCheck{ApplicabilityCheckType::Level, FlowSdk::FlowMeasure::At, 350,
                                                          350, true},
                            LevelFilterApplicabilityCheck{ApplicabilityCheckType::Level, FlowSdk::FlowMeasure::At, 350,
                                                          340, false},
                            LevelFilterApplicabilityCheck{ApplicabilityCheckType::Level, FlowSdk::FlowMeasure::At, 350,
                                                          360, false},
                            LevelFilterApplicabilityCheck{ApplicabilityCheckType::Level,
                                                          FlowSdk::FlowMeasure::AtOrBelow, 350, 350, true},
                            LevelFilterApplicabilityCheck{ApplicabilityCheckType::Level,
                                                          FlowSdk::FlowMeasure::AtOrBelow, 350, 340, true},
                            LevelFilterApplicabilityCheck{ApplicabilityCheckType::Level,
                                                          FlowSdk::FlowMeasure::AtOrBelow, 350, 360, false},
                            LevelFilterApplicabilityCheck{ApplicabilityCheckType::Level,
                                                          FlowSdk::FlowMeasure::AtOrAbove, 350, 350, true},
                            LevelFilterApplicabilityCheck{ApplicabilityCheckType::Level,
                                                          FlowSdk::FlowMeasure::AtOrAbove, 350, 360, true},
                            LevelFilterApplicabilityCheck{ApplicabilityCheckType::Level,
                                                          FlowSdk::FlowMeasure::AtOrAbove, 350, 340, false},
                            LevelFilterApplicabilityCheck{ApplicabilityCheckType::Altitude, FlowSdk::FlowMeasure::At,
                                                          350, 35000, true},
                            LevelFilterApplicabilityCheck{ApplicabilityCheckType::Altitude, FlowSdk::FlowMeasure::At,
                                                          350, 34000, false},
                            LevelFilterApplicabilityCheck{ApplicabilityCheckType::Altitude, FlowSdk::FlowMeasure::At,
                                                          350, 36000, false},
                            LevelFilterApplicabilityCheck{ApplicabilityCheckType::Altitude,
                                                          FlowSdk::FlowMeasure::AtOrBelow, 350, 35000, true},
                            LevelFilterApplicabilityCheck{ApplicabilityCheckType::Altitude,
                                                          FlowSdk::FlowMeasure::AtOrBelow, 350, 34000, true},
                            LevelFilterApplicabilityCheck{ApplicabilityCheckType::Altitude,
                                                          FlowSdk::FlowMeasure::AtOrBelow, 350, 36000, false},
                            LevelFilterApplicabilityCheck{ApplicabilityCheckType::Altitude,
                                                          FlowSdk::FlowMeasure::AtOrAbove, 350, 35000, true},
                            LevelFilterApplicabilityCheck{ApplicabilityCheckType::Altitude,
                                                          FlowSdk::FlowMeasure::AtOrAbove, 350, 36000, true},
                            LevelFilterApplicabilityCheck{ApplicabilityCheckType::Altitude,
                                                          FlowSdk::FlowMeasure::AtOrAbove, 350, 34000, false}),
            [](const ::testing::TestParamInfo<ConcreteLevelFilterApplicabilityTest::ParamType>& info) {
                std::string name;
                switch (info.param.filterType) {
                case FlowSdk::FlowMeasure::At: {
                    name += "At_";
                    break;
                }
                case FlowSdk::FlowMeasure::AtOrBelow: {
                    name += "AtOrBelow_";
                    break;
                }
                case FlowSdk::FlowMeasure::AtOrAbove: {
                    name += "AtOrAbove_";
                    break;
                }
                }

                name += std::to_string(info.param.filterLevel) + "_with_";

                if (info.param.checkType == Level) {
                    name += "level_";
                }
                else {
                    name += "altitude_";
                }
                name += std::to_string(info.param.checkLevel);

                return name;
            });
}// namespace FlowSdkTest::FlowMeasure
