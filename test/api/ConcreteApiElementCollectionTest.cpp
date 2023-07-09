#include "api/ConcreteApiElementCollection.h"
#include "flightinformationregion/ConcreteFlightInformationRegion.h"

namespace ECFMPTest::Api {
    class ConcreteApiElementCollectionTest : public testing::Test
    {
        public:
        ConcreteApiElementCollectionTest()
            : fir1(std::make_shared<ECFMP::FlightInformationRegion::ConcreteFlightInformationRegion>(
                    1, "EGTT", "London"
            )),
              fir2(std::make_shared<ECFMP::FlightInformationRegion::ConcreteFlightInformationRegion>(
                      2, "EGPX", "Scottish"
              )),
              fir3(std::make_shared<ECFMP::FlightInformationRegion::ConcreteFlightInformationRegion>(
                      3, "EHAA", "Amsterdam"
              ))
        {}

        std::shared_ptr<ECFMP::FlightInformationRegion::FlightInformationRegion> fir1;
        std::shared_ptr<ECFMP::FlightInformationRegion::FlightInformationRegion> fir2;
        std::shared_ptr<ECFMP::FlightInformationRegion::FlightInformationRegion> fir3;
        ECFMP::Api::ConcreteApiElementCollection<ECFMP::FlightInformationRegion::FlightInformationRegion> firs;
    };

    TEST_F(ConcreteApiElementCollectionTest, ItAddsElements)
    {
        firs.Add(fir1);
        firs.Add(fir2);
        firs.Add(fir3);

        EXPECT_EQ(fir1, firs.Get(1));
        EXPECT_EQ(fir2, firs.Get(2));
        EXPECT_EQ(fir3, firs.Get(3));
        EXPECT_EQ(3, firs.Count());
    }

    TEST_F(ConcreteApiElementCollectionTest, ItReplacesElements)
    {
        const auto extra = std::make_shared<ECFMP::FlightInformationRegion::ConcreteFlightInformationRegion>(
                3, "EHAA", "Amsterdam 2"
        );

        firs.Add(fir1);
        firs.Add(fir2);
        firs.Add(fir3);
        firs.Add(extra);

        EXPECT_EQ(3, firs.Count());
        EXPECT_EQ(extra, firs.Get(3));
    }

    TEST_F(ConcreteApiElementCollectionTest, ItReturnsNullptrElementNotFound)
    {
        firs.Add(fir1);
        firs.Add(fir2);
        firs.Add(fir3);

        EXPECT_EQ(nullptr, firs.Get(4));
    }

    TEST_F(ConcreteApiElementCollectionTest, ItCanBeIterated)
    {
        std::list<int> iteratedFirs;

        firs.Add(fir1);
        firs.Add(fir2);
        firs.Add(fir3);

        for (auto& fir: firs) {
            iteratedFirs.push_back(fir.Id());
        }

        EXPECT_EQ(std::list<int>({1, 2, 3}), iteratedFirs);
    }

    TEST_F(ConcreteApiElementCollectionTest, ItCanBeConstIterated)
    {
        std::list<int> iteratedFirs;

        firs.Add(fir1);
        firs.Add(fir2);
        firs.Add(fir3);

        for (const auto& fir: firs) {
            iteratedFirs.push_back(fir.Id());
        }

        EXPECT_EQ(std::list<int>({1, 2, 3}), iteratedFirs);
    }
}// namespace ECFMPTest::Api
