#include "flightinformationregion/ConcreteFlightInformationRegion.h"

namespace ECFMPTest::FlightInformationRegion {
    class ConcreteFlightInformationRegionTest : public testing::Test
    {
        public:
        ConcreteFlightInformationRegionTest() : fir(1, "EGTT", "London")
        {}

        ECFMP::FlightInformationRegion::ConcreteFlightInformationRegion fir;
    };

    TEST_F(ConcreteFlightInformationRegionTest, ItHasAnId)
    {
        EXPECT_EQ(1, fir.Id());
    }

    TEST_F(ConcreteFlightInformationRegionTest, ItHasAnIdentifier)
    {
        EXPECT_EQ("EGTT", fir.Identifier());
    }

    TEST_F(ConcreteFlightInformationRegionTest, ItHasAName)
    {
        EXPECT_EQ("London", fir.Name());
    }
}// namespace ECFMPTest::FlightInformationRegion
