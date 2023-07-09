#include "flowmeasure/ConcreteLevelRangeFilter.h"

namespace ECFMPTest::FlowMeasure {

    enum ApplicabilityCheckType
    {
        Altitude = 0,
        Level = 1,
    };

    using LevelFilterApplicabilityCheck = struct LevelFilterApplicabilityCheck {
        // The type of check
        ApplicabilityCheckType checkType;

        // The filter type
        ECFMP::FlowMeasure::LevelRangeFilterType filterType;

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
        ConcreteLevelFilterTest() : levelFilter(ECFMP::FlowMeasure::LevelRangeFilterType::AtOrBelow, 350)
        {}

        ECFMP::FlowMeasure::ConcreteLevelRangeFilter levelFilter;
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
        EXPECT_EQ(ECFMP::FlowMeasure::LevelRangeFilterType::AtOrBelow, levelFilter.Type());
    }

    class ConcreteLevelFilterApplicabilityTest : public testing::TestWithParam<LevelFilterApplicabilityCheck>
    {
    };

    TEST_P(ConcreteLevelFilterApplicabilityTest, ItChecksAirportApplicability)
    {
        ECFMP::FlowMeasure::ConcreteLevelRangeFilter filter(GetParam().filterType, GetParam().filterLevel);
        if (GetParam().checkType == ApplicabilityCheckType::Level) {
            EXPECT_EQ(GetParam().expectedApplicability, filter.ApplicableToLevel(GetParam().checkLevel));
        }
        else {
            EXPECT_EQ(GetParam().expectedApplicability, filter.ApplicableToAltitude(GetParam().checkLevel));
        }
    }

    INSTANTIATE_TEST_SUITE_P(
            ConcreteLevelFilterApplicabilityTestCases, ConcreteLevelFilterApplicabilityTest,
            testing::Values(
                    LevelFilterApplicabilityCheck{
                            ApplicabilityCheckType::Level, ECFMP::FlowMeasure::AtOrBelow, 350, 350, true},
                    LevelFilterApplicabilityCheck{
                            ApplicabilityCheckType::Level, ECFMP::FlowMeasure::AtOrBelow, 350, 340, true},
                    LevelFilterApplicabilityCheck{
                            ApplicabilityCheckType::Level, ECFMP::FlowMeasure::AtOrBelow, 350, 360, false},
                    LevelFilterApplicabilityCheck{
                            ApplicabilityCheckType::Level, ECFMP::FlowMeasure::AtOrAbove, 350, 350, true},
                    LevelFilterApplicabilityCheck{
                            ApplicabilityCheckType::Level, ECFMP::FlowMeasure::AtOrAbove, 350, 360, true},
                    LevelFilterApplicabilityCheck{
                            ApplicabilityCheckType::Level, ECFMP::FlowMeasure::AtOrAbove, 350, 340, false},
                    LevelFilterApplicabilityCheck{
                            ApplicabilityCheckType::Altitude, ECFMP::FlowMeasure::AtOrBelow, 350, 35000, true},
                    LevelFilterApplicabilityCheck{
                            ApplicabilityCheckType::Altitude, ECFMP::FlowMeasure::AtOrBelow, 350, 34000, true},
                    LevelFilterApplicabilityCheck{
                            ApplicabilityCheckType::Altitude, ECFMP::FlowMeasure::AtOrBelow, 350, 36000, false},
                    LevelFilterApplicabilityCheck{
                            ApplicabilityCheckType::Altitude, ECFMP::FlowMeasure::AtOrAbove, 350, 35000, true},
                    LevelFilterApplicabilityCheck{
                            ApplicabilityCheckType::Altitude, ECFMP::FlowMeasure::AtOrAbove, 350, 36000, true},
                    LevelFilterApplicabilityCheck{
                            ApplicabilityCheckType::Altitude, ECFMP::FlowMeasure::AtOrAbove, 350, 34000, false}
            ),
            [](const ::testing::TestParamInfo<ConcreteLevelFilterApplicabilityTest::ParamType>& info) {
                std::string name;
                switch (info.param.filterType) {
                case ECFMP::FlowMeasure::AtOrBelow: {
                    name += "AtOrBelow_";
                    break;
                }
                case ECFMP::FlowMeasure::AtOrAbove: {
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
            }
    );
}// namespace ECFMPTest::FlowMeasure
