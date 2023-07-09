#include "flowmeasure/ConcreteMultipleLevelFilter.h"

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
        ASSERT_EQ(filter.Levels().size(), 3);
        ASSERT_EQ(filter.Levels(), expected);
    }

    TEST_F(ConcreteMultipleFlowLevelFilterTest, ItReturnsAltitudes)
    {
        auto expected = std::vector<int>{34000, 36000, 39000};
        ASSERT_EQ(filter.Altitudes().size(), 3);
        ASSERT_EQ(filter.Altitudes(), expected);
    }

    TEST_F(ConcreteMultipleFlowLevelFilterTest, ItReturnsTrueForApplicableLevel)
    {
        ASSERT_TRUE(filter.ApplicableToLevel(340));
        ASSERT_TRUE(filter.ApplicableToLevel(360));
        ASSERT_TRUE(filter.ApplicableToLevel(390));
    }

    TEST_F(ConcreteMultipleFlowLevelFilterTest, ItReturnsFalseForNonApplicableLevel)
    {
        ASSERT_FALSE(filter.ApplicableToLevel(350));
        ASSERT_FALSE(filter.ApplicableToLevel(370));
        ASSERT_FALSE(filter.ApplicableToLevel(380));
    }

    TEST_F(ConcreteMultipleFlowLevelFilterTest, ItReturnsTrueForApplicableAltitude)
    {
        ASSERT_TRUE(filter.ApplicableToAltitude(34000));
        ASSERT_TRUE(filter.ApplicableToAltitude(36000));
        ASSERT_TRUE(filter.ApplicableToAltitude(39000));
    }

    TEST_F(ConcreteMultipleFlowLevelFilterTest, ItReturnsFalseForNonApplicableAltitude)
    {
        ASSERT_FALSE(filter.ApplicableToAltitude(35000));
        ASSERT_FALSE(filter.ApplicableToAltitude(37000));
        ASSERT_FALSE(filter.ApplicableToAltitude(38000));
    }
}// namespace ECFMPTest::FlowMeasure
