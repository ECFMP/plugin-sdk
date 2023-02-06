#include "flowmeasure/ConcreteRouteFilter.h"

namespace FlowSdkTest::FlowMeasure {

    class ConcreteRouteFilterTest : public testing::Test
    {
        public:
        ConcreteRouteFilterTest() : routeFilter(std::set<std::string>{"ABC", "DEF", "GHI"})
        {}

        FlowSdk::FlowMeasure::ConcreteRouteFilter routeFilter;
    };

    TEST_F(ConcreteRouteFilterTest, ItReturnsRoutes)
    {
        EXPECT_EQ(std::set<std::string>({"ABC", "DEF", "GHI"}), routeFilter.RouteStrings());
    }
}// namespace FlowSdkTest::FlowMeasure
