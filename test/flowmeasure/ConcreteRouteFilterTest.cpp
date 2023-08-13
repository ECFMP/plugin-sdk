#include "flowmeasure/ConcreteRouteFilter.h"
#include "mock/MockEuroscopeAircraft.h"

namespace ECFMPTest::FlowMeasure {

    class ConcreteRouteFilterTest : public testing::Test
    {
        public:
        ConcreteRouteFilterTest() : routeFilter(std::set<std::string>{"ABC 123", "DEF", "GHI"})
        {}

        ECFMP::FlowMeasure::ConcreteRouteFilter routeFilter;
    };

    TEST_F(ConcreteRouteFilterTest, ItReturnsRoutes)
    {
        EXPECT_EQ(std::set<std::string>({"ABC 123", "DEF", "GHI"}), routeFilter.RouteStrings());
    }

    TEST_F(ConcreteRouteFilterTest, ItsApplicableToAircraftIfRouteStringIsFound)
    {
        testing::NiceMock<Euroscope::MockEuroscopeAircraft> aircraft;
        EXPECT_CALL(aircraft, RouteString).WillOnce(testing::Return("LOL ABC 123 LOL"));
        EXPECT_TRUE(routeFilter.ApplicableToAircraft(aircraft));
    }

    TEST_F(ConcreteRouteFilterTest, ItsNotApplicableToAircraftIfRouteStringIsNotFound)
    {
        testing::NiceMock<Euroscope::MockEuroscopeAircraft> aircraft;
        EXPECT_CALL(aircraft, RouteString).WillRepeatedly(testing::Return("LOL ADG 123 LOL"));
        EXPECT_FALSE(routeFilter.ApplicableToAircraft(aircraft));
    }

    TEST_F(ConcreteRouteFilterTest, ItReturnsFilterDescription)
    {
        EXPECT_EQ("On route(s): ABC 123, DEF, GHI", routeFilter.FilterDescription());
    }
}// namespace ECFMPTest::FlowMeasure
