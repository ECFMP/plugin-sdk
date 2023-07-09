#include "flowmeasure/ConcreteRouteFilter.h"

namespace ECFMPTest::FlowMeasure {

    class ConcreteRouteFilterTest : public testing::Test
    {
        public:
        ConcreteRouteFilterTest() : routeFilter(std::set<std::string>{"ABC", "DEF", "GHI"})
        {}

        ECFMP::FlowMeasure::ConcreteRouteFilter routeFilter;
    };

    TEST_F(ConcreteRouteFilterTest, ItReturnsRoutes)
    {
        EXPECT_EQ(std::set<std::string>({"ABC", "DEF", "GHI"}), routeFilter.RouteStrings());
    }
}// namespace ECFMPTest::FlowMeasure
