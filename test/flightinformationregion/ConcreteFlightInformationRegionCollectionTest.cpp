#include "flightinformationregion/ConcreteFlightInformationRegionCollection.h"
#include "flightinformationregion/ConcreteFlightInformationRegion.h"

namespace FlowSdkTest::FlightInformationRegion {
    class ConcreteFlightInformationRegionCollectionTest : public testing::Test
    {
        public:
        ConcreteFlightInformationRegionCollectionTest()
            : fir1(std::make_shared<FlowSdk::FlightInformationRegion::ConcreteFlightInformationRegion>(1, "EGTT",
                                                                                                       "London")),
              fir2(std::make_shared<FlowSdk::FlightInformationRegion::ConcreteFlightInformationRegion>(2, "EGPX",
                                                                                                       "Scottish")),
              fir3(std::make_shared<FlowSdk::FlightInformationRegion::ConcreteFlightInformationRegion>(1, "EGGX",
                                                                                                       "Shanwick"))
        {}

        std::shared_ptr<FlowSdk::FlightInformationRegion::ConcreteFlightInformationRegion> fir1;
        std::shared_ptr<FlowSdk::FlightInformationRegion::ConcreteFlightInformationRegion> fir2;
        std::shared_ptr<FlowSdk::FlightInformationRegion::ConcreteFlightInformationRegion> fir3;
        FlowSdk::FlightInformationRegion::ConcreteFlightInformationRegionCollection firs;
    };

    TEST_F(ConcreteFlightInformationRegionCollectionTest, ItGetsById)
    {
        firs.Add(fir1);
        firs.Add(fir2);

        EXPECT_EQ(fir1, firs.Get(1));
    }

    TEST_F(ConcreteFlightInformationRegionCollectionTest, ItReturnsNullptrIfNotFoundById)
    {
        firs.Add(fir1);
        firs.Add(fir2);

        EXPECT_EQ(nullptr, firs.Get(3));
    }

    TEST_F(ConcreteFlightInformationRegionCollectionTest, ItGetsByIdentifier)
    {
        firs.Add(fir1);
        firs.Add(fir2);

        EXPECT_EQ(fir1, firs.Get("EGTT"));
    }

    TEST_F(ConcreteFlightInformationRegionCollectionTest, ItReturnsNullptrIfNotFoundByIdentifier)
    {
        firs.Add(fir1);
        firs.Add(fir2);

        EXPECT_EQ(nullptr, firs.Get("EXXY"));
    }

    TEST_F(ConcreteFlightInformationRegionCollectionTest, ItReplacesById)
    {
        firs.Add(fir1);
        firs.Add(fir2);
        firs.Add(fir3);

        EXPECT_EQ(fir3, firs.Get(1));
    }
}// namespace FlowSdkTest::FlightInformationRegion
