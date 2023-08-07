#include "flowmeasure/ConcreteRouteFilter.h"
#include "mock/MockEuroscopeAircraft.h"

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

    TEST_F(ConcreteRouteFilterTest, ItsApplicableToAircraftIfRouteStringIsFound)
    {
        testing::NiceMock<Euroscope::MockEuroscopeAircraft> aircraft;
        EXPECT_CALL(aircraft, RouteString).WillOnce(testing::Return("LOL ABC LOL"));
        EXPECT_TRUE(routeFilter.ApplicableToAircraft(aircraft));
    }

    TEST_F(ConcreteRouteFilterTest, ItsNotApplicableToAircraftIfRouteStringIsNotFound)
    {
        testing::NiceMock<Euroscope::MockEuroscopeAircraft> aircraft;
        EXPECT_CALL(aircraft, RouteString).WillRepeatedly(testing::Return("LOL ADG LOL"));
        EXPECT_FALSE(routeFilter.ApplicableToAircraft(aircraft));
    }
}// namespace ECFMPTest::FlowMeasure
