#include "flowmeasure/ConcreteRangeToDestinationFilter.h"

namespace FlowSdkTest::FlowMeasure {
    class ConcreteRangeToDestinationFilterTest : public ::testing::Test
    {
        public:
        FlowSdk::FlowMeasure::ConcreteRangeToDestinationFilter filter{123};
    };

    TEST_F(ConcreteRangeToDestinationFilterTest, RangeReturnsRange)
    {
        ASSERT_EQ(filter.Range(), 123);
    }
}// namespace FlowSdkTest::FlowMeasure
