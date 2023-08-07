#include "flowmeasure/ConcreteMultipleLevelFilter.h"
#include "mock/MockEuroscopeAircraft.h"

namespace ECFMPTest::FlowMeasure {

    class ConcreteMultipleFlowLevelFilterTest : public testing::Test
    {
        public:
        ConcreteMultipleFlowLevelFilterTest() : filter(std::vector<int>{340, 360, 390})
        {}

        ECFMP::FlowMeasure::ConcreteMultipleLevelFilter filter;
    };

    TEST_F(ConcreteMultipleFlowLevelFilterTest, ItReturnsLevels)
    {
        auto expected = std::vector<int>{340, 360, 390};
        EXPECT_EQ(filter.Levels().size(), 3);
        EXPECT_EQ(filter.Levels(), expected);
    }

    TEST_F(ConcreteMultipleFlowLevelFilterTest, ItReturnsAltitudes)
    {
        auto expected = std::vector<int>{34000, 36000, 39000};
        EXPECT_EQ(filter.Altitudes().size(), 3);
        EXPECT_EQ(filter.Altitudes(), expected);
    }

    TEST_F(ConcreteMultipleFlowLevelFilterTest, ItReturnsTrueForApplicableLevel)
    {
        EXPECT_TRUE(filter.ApplicableToLevel(340));
        EXPECT_TRUE(filter.ApplicableToLevel(360));
        EXPECT_TRUE(filter.ApplicableToLevel(390));
    }

    TEST_F(ConcreteMultipleFlowLevelFilterTest, ItReturnsFalseForNonApplicableLevel)
    {
        EXPECT_FALSE(filter.ApplicableToLevel(350));
        EXPECT_FALSE(filter.ApplicableToLevel(370));
        EXPECT_FALSE(filter.ApplicableToLevel(380));
    }

    TEST_F(ConcreteMultipleFlowLevelFilterTest, ItReturnsTrueForApplicableAltitude)
    {
        EXPECT_TRUE(filter.ApplicableToAltitude(34000));
        EXPECT_TRUE(filter.ApplicableToAltitude(36000));
        EXPECT_TRUE(filter.ApplicableToAltitude(39000));
    }

    TEST_F(ConcreteMultipleFlowLevelFilterTest, ItReturnsFalseForNonApplicableAltitude)
    {
        EXPECT_FALSE(filter.ApplicableToAltitude(35000));
        EXPECT_FALSE(filter.ApplicableToAltitude(37000));
        EXPECT_FALSE(filter.ApplicableToAltitude(38000));
    }

    TEST_F(ConcreteMultipleFlowLevelFilterTest, ItReturnsTrueForApplicableAltitudeForAircraft)
    {
        const auto aircraft = testing::NiceMock<Euroscope::MockEuroscopeAircraft>();
        ON_CALL(aircraft, CruiseAltitude).WillByDefault(testing::Return(34000));
        EXPECT_TRUE(filter.ApplicableToAircraft(aircraft));
    }

    TEST_F(ConcreteMultipleFlowLevelFilterTest, ItReturnsFalseForNonApplicableAltitudeForAircraft)
    {
        const auto aircraft = testing::NiceMock<Euroscope::MockEuroscopeAircraft>();
        ON_CALL(aircraft, CruiseAltitude).WillByDefault(testing::Return(35000));
        EXPECT_FALSE(filter.ApplicableToAircraft(aircraft));
    }
}// namespace ECFMPTest::FlowMeasure
