#include "flowmeasure/ConcreteRangeToDestinationFilter.h"
#include "mock/MockEuroscopeAircraft.h"

namespace ECFMPTest::FlowMeasure {
    class ConcreteRangeToDestinationFilterTest : public ::testing::Test
    {
        public:
        ECFMP::FlowMeasure::ConcreteRangeToDestinationFilter filter{123};
    };

    TEST_F(ConcreteRangeToDestinationFilterTest, RangeReturnsRange)
    {
        EXPECT_EQ(filter.Range(), 123);
    }

    TEST_F(ConcreteRangeToDestinationFilterTest, FilterReturnsTrueIfApplicableToAircraftEqual)
    {
        testing::NiceMock<Euroscope::MockEuroscopeAircraft> aircraft;
        EXPECT_CALL(aircraft, RangeToDestination).WillOnce(testing::Return(123.0));
        EXPECT_TRUE(filter.ApplicableToAircraft(aircraft));
    }

    TEST_F(ConcreteRangeToDestinationFilterTest, FilterReturnsTrueIfApplicableToAircraftLessThan)
    {
        testing::NiceMock<Euroscope::MockEuroscopeAircraft> aircraft;
        EXPECT_CALL(aircraft, RangeToDestination).WillOnce(testing::Return(121.0));
        EXPECT_TRUE(filter.ApplicableToAircraft(aircraft));
    }

    TEST_F(ConcreteRangeToDestinationFilterTest, FilterReturnsFalseIfApplicableToAircraftMoreThan)
    {
        testing::NiceMock<Euroscope::MockEuroscopeAircraft> aircraft;
        EXPECT_CALL(aircraft, RangeToDestination).WillOnce(testing::Return(123.1));
        EXPECT_FALSE(filter.ApplicableToAircraft(aircraft));
    }

    TEST_F(ConcreteRangeToDestinationFilterTest, FilterDescriptionReturnsCorrectString)
    {
        EXPECT_EQ(filter.FilterDescription(), "Range to Destination Less Than: 123nm");
    }
}// namespace ECFMPTest::FlowMeasure
