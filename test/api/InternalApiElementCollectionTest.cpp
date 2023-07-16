#include "api/InternalApiElementCollection.h"
#include "flightinformationregion/ConcreteFlightInformationRegion.h"

namespace ECFMPTest::Api {
    class InternalApiElementCollectionTest : public testing::Test
    {
        public:
        InternalApiElementCollectionTest()
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
        ECFMP::Api::InternalApiElementCollection<ECFMP::FlightInformationRegion::FlightInformationRegion> firs;
    };

    TEST_F(InternalApiElementCollectionTest, ItAddsElements)
    {
        firs.Add(fir1);
        firs.Add(fir2);
        firs.Add(fir3);

        EXPECT_EQ(fir1, firs.Get(1));
        EXPECT_EQ(fir2, firs.Get(2));
        EXPECT_EQ(fir3, firs.Get(3));
        EXPECT_EQ(3, firs.Count());
    }

    TEST_F(InternalApiElementCollectionTest, ItReplacesElements)
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

    TEST_F(InternalApiElementCollectionTest, ItReturnsNullptrElementNotFound)
    {
        firs.Add(fir1);
        firs.Add(fir2);
        firs.Add(fir3);

        EXPECT_EQ(nullptr, firs.Get(4));
    }

    TEST_F(InternalApiElementCollectionTest, ItCanBeIterated)
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

    TEST_F(InternalApiElementCollectionTest, ItCanBeConstIterated)
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

    TEST_F(InternalApiElementCollectionTest, ItCanBeCounted)
    {
        firs.Add(fir1);
        firs.Add(fir2);
        firs.Add(fir3);

        EXPECT_EQ(3, firs.Count());
    }

    TEST_F(InternalApiElementCollectionTest, ItContainsAnElementById)
    {
        firs.Add(fir1);
        firs.Add(fir2);
        firs.Add(fir3);

        EXPECT_TRUE(firs.Contains(1));
        EXPECT_TRUE(firs.Contains(2));
        EXPECT_TRUE(firs.Contains(3));
        EXPECT_FALSE(firs.Contains(4));
    }

    TEST_F(InternalApiElementCollectionTest, ItContainsAnElementByPredicate)
    {
        firs.Add(fir1);
        firs.Add(fir2);
        firs.Add(fir3);

        EXPECT_TRUE(firs.Contains([](const auto& fir) {
            return fir.Id() == 1;
        }));
        EXPECT_TRUE(firs.Contains([](const auto& fir) {
            return fir.Id() == 2;
        }));
        EXPECT_TRUE(firs.Contains([](const auto& fir) {
            return fir.Id() == 3;
        }));
        EXPECT_FALSE(firs.Contains([](const auto& fir) {
            return fir.Id() == 4;
        }));
    }

    TEST_F(InternalApiElementCollectionTest, ItReturnsFirstElementByPredicate)
    {
        firs.Add(fir1);
        firs.Add(fir2);
        firs.Add(fir3);

        EXPECT_EQ(fir1, firs.First([](const auto& fir) {
            return fir.Id() == 1;
        }));
        EXPECT_EQ(fir2, firs.First([](const auto& fir) {
            return fir.Id() == 2;
        }));
        EXPECT_EQ(fir3, firs.First([](const auto& fir) {
            return fir.Id() == 3;
        }));
        EXPECT_EQ(nullptr, firs.First([](const auto& fir) {
            return fir.Id() == 4;
        }));
    }
}// namespace ECFMPTest::Api
