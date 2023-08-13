#include "flowmeasure/ConcreteLevelRangeFilter.h"
#include "mock/MockEuroscopeAircraft.h"

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

    TEST_F(ConcreteLevelFilterTest, ItReturnsFilterDescriptionAtOrBelow)
    {
        EXPECT_EQ("At or below: FL350", levelFilter.FilterDescription());
    }

    TEST_F(ConcreteLevelFilterTest, ItReturnsFilterDescriptionAtOrAbove)
    {
        ECFMP::FlowMeasure::ConcreteLevelRangeFilter filter(ECFMP::FlowMeasure::LevelRangeFilterType::AtOrAbove, 350);
        EXPECT_EQ("At or above: FL350", filter.FilterDescription());
    }

    class ConcreteLevelFilterApplicabilityTest : public testing::TestWithParam<LevelFilterApplicabilityCheck>
    {
    };

    TEST_P(ConcreteLevelFilterApplicabilityTest, ItChecksApplicability)
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

    using LevelFilterAircraftApplicabilityCheck = struct LevelFilterAircraftApplicabilityCheck {
        // The filter type
        ECFMP::FlowMeasure::LevelRangeFilterType filterType;

        // The level for the filter
        int filterLevel;

        // The level to check against
        int checkLevel;

        // Expected applicability
        bool expectedApplicability;
    };

    class ConcreteLevelFilterAircraftApplicabilityTest
        : public testing::TestWithParam<LevelFilterAircraftApplicabilityCheck>
    {
    };

    TEST_P(ConcreteLevelFilterAircraftApplicabilityTest, ItChecksApplicabilityForAircraft)
    {
        ECFMP::FlowMeasure::ConcreteLevelRangeFilter filter(GetParam().filterType, GetParam().filterLevel);
        const auto aircraft = testing::NiceMock<Euroscope::MockEuroscopeAircraft>();
        ON_CALL(aircraft, CruiseAltitude).WillByDefault(testing::Return(GetParam().checkLevel));
        EXPECT_EQ(GetParam().expectedApplicability, filter.ApplicableToAircraft(aircraft));
    }

    INSTANTIATE_TEST_SUITE_P(
            ConcreteLevelFilterAircraftApplicabilityTestCases, ConcreteLevelFilterAircraftApplicabilityTest,
            testing::Values(
                    LevelFilterAircraftApplicabilityCheck{ECFMP::FlowMeasure::AtOrBelow, 350, 35000, true},
                    LevelFilterAircraftApplicabilityCheck{ECFMP::FlowMeasure::AtOrBelow, 350, 34000, true},
                    LevelFilterAircraftApplicabilityCheck{ECFMP::FlowMeasure::AtOrBelow, 350, 36000, false},
                    LevelFilterAircraftApplicabilityCheck{ECFMP::FlowMeasure::AtOrAbove, 350, 35000, true},
                    LevelFilterAircraftApplicabilityCheck{ECFMP::FlowMeasure::AtOrAbove, 350, 36000, true},
                    LevelFilterAircraftApplicabilityCheck{ECFMP::FlowMeasure::AtOrAbove, 350, 34000, false}
            ),
            [](const ::testing::TestParamInfo<ConcreteLevelFilterAircraftApplicabilityTest::ParamType>& info) {
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

                name += std::to_string(info.param.filterLevel) + "_with_altitude_";
                name += std::to_string(info.param.checkLevel);

                return name;
            }
    );
}// namespace ECFMPTest::FlowMeasure
