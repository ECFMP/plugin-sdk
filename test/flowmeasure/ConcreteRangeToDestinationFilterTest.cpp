#include "flowmeasure/ConcreteRangeToDestinationFilter.h"

namespace ECFMPTest::FlowMeasure {
    class ConcreteRangeToDestinationFilterTest : public ::testing::Test
    {
        public:
        ECFMP::FlowMeasure::ConcreteRangeToDestinationFilter filter{123};
    };

    TEST_F(ConcreteRangeToDestinationFilterTest, RangeReturnsRange)
    {
        ASSERT_EQ(filter.Range(), 123);
    }
}// namespace ECFMPTest::FlowMeasure
