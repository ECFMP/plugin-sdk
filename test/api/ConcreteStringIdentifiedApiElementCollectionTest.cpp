#include "api/ConcreteStringIdentifiedApiElementCollection.h"
#include "flightinformationregion/ConcreteFlightInformationRegion.h"

namespace FlowSdkTest::Api {
    class ConcreteStringIdentifiedApiElementCollectionTest : public testing::Test
    {
        public:
        ConcreteStringIdentifiedApiElementCollectionTest()
            : fir1(std::make_shared<FlowSdk::FlightInformationRegion::ConcreteFlightInformationRegion>(
                    1, "EGTT", "London"
            )),
              fir2(std::make_shared<FlowSdk::FlightInformationRegion::ConcreteFlightInformationRegion>(
                      2, "EGPX", "Scottish"
              )),
              fir3(std::make_shared<FlowSdk::FlightInformationRegion::ConcreteFlightInformationRegion>(
                      3, "EHAA", "Amsterdam"
              ))
        {}

        std::shared_ptr<FlowSdk::FlightInformationRegion::FlightInformationRegion> fir1;
        std::shared_ptr<FlowSdk::FlightInformationRegion::FlightInformationRegion> fir2;
        std::shared_ptr<FlowSdk::FlightInformationRegion::FlightInformationRegion> fir3;
        FlowSdk::Api::ConcreteStringIdentifiedApiElementCollection<
                FlowSdk::FlightInformationRegion::FlightInformationRegion>
                firs;
    };

    TEST_F(ConcreteStringIdentifiedApiElementCollectionTest, ItAddsElements)
    {
        firs.Add(fir1);
        firs.Add(fir2);
        firs.Add(fir3);

        EXPECT_EQ(fir1, firs.Get(1));
        EXPECT_EQ(fir2, firs.Get(2));
        EXPECT_EQ(fir3, firs.Get(3));
        EXPECT_EQ(3, firs.Count());
    }

    TEST_F(ConcreteStringIdentifiedApiElementCollectionTest, ItReplacesElements)
    {
        const auto extra = std::make_shared<FlowSdk::FlightInformationRegion::ConcreteFlightInformationRegion>(
                3, "EHAA", "Amsterdam 2"
        );

        firs.Add(fir1);
        firs.Add(fir2);
        firs.Add(fir3);
        firs.Add(extra);

        EXPECT_EQ(3, firs.Count());
        EXPECT_EQ(extra, firs.Get(3));
    }

    TEST_F(ConcreteStringIdentifiedApiElementCollectionTest, ItReturnsNullptrElementNotFound)
    {
        firs.Add(fir1);
        firs.Add(fir2);
        firs.Add(fir3);

        EXPECT_EQ(nullptr, firs.Get(4));
    }

    TEST_F(ConcreteStringIdentifiedApiElementCollectionTest, ItReturnsElementsByIdentifier)
    {
        firs.Add(fir1);
        firs.Add(fir2);
        firs.Add(fir3);

        EXPECT_EQ(fir2, firs.GetByIdentifier("EGPX"));
    }

    TEST_F(ConcreteStringIdentifiedApiElementCollectionTest, ItReturnsNullptrElementsNotFoundByIdentifier)
    {
        firs.Add(fir1);
        firs.Add(fir2);
        firs.Add(fir3);

        EXPECT_EQ(nullptr, firs.GetByIdentifier("ABCD"));
    }

    TEST_F(ConcreteStringIdentifiedApiElementCollectionTest, ItCanBeIterated)
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

    TEST_F(ConcreteStringIdentifiedApiElementCollectionTest, ItCanBeConstIterated)
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
}// namespace FlowSdkTest::Api
