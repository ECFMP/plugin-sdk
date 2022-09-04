#include "flightinformationregion/ConcreteFlightInformationRegion.h"
#include "gtest/gtest.h"

namespace FlowSdkTest::FlightInformationRegion {
    class ConcreteFlightInformationRegionTest : public testing::Test
    {
        public:
        ConcreteFlightInformationRegionTest() : fir(1, "EGTT", "London")
        {}

        FlowSdk::FlightInformationRegion::ConcreteFlightInformationRegion fir;
    };

    TEST_F(ConcreteFlightInformationRegionTest, ItHasAnId)
    {
        EXPECT_EQ(1, fir.Id());
    }

    TEST_F(ConcreteFlightInformationRegionTest, ItHasAnIdentifier)
    {
        EXPECT_EQ(0, strcmp("EGTT", fir.Identifier()));
    }

    TEST_F(ConcreteFlightInformationRegionTest, ItHasAName)
    {
        EXPECT_EQ(0, strcmp("London", fir.Name()));
    }
}// namespace FlowSdkTest::FlightInformationRegion
