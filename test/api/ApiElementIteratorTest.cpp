#include "flow-sdk/ApiElementIterator.h"
#include "flightinformationregion/ConcreteFlightInformationRegion.h"

namespace FlowSdkTest::Api {
    class ApiElementIteratorTest : public testing::Test
    {
        public:
        ApiElementIteratorTest()
            : fir1(std::make_shared<FlowSdk::FlightInformationRegion::ConcreteFlightInformationRegion>(
                    1, "EGTT", "London"
            )),
              fir2(std::make_shared<FlowSdk::FlightInformationRegion::ConcreteFlightInformationRegion>(
                      2, "EGPX", "Scottish"
              )),
              fir3(std::make_shared<FlowSdk::FlightInformationRegion::ConcreteFlightInformationRegion>(
                      3, "EHAA", "Amsterdam"
              )),
              map({{1, fir1}, {2, fir2}, {3, fir3}})
        {}

        std::recursive_mutex mutex;
        std::shared_ptr<FlowSdk::FlightInformationRegion::FlightInformationRegion> fir1;
        std::shared_ptr<FlowSdk::FlightInformationRegion::FlightInformationRegion> fir2;
        std::shared_ptr<FlowSdk::FlightInformationRegion::FlightInformationRegion> fir3;
        std::map<int, std::shared_ptr<FlowSdk::FlightInformationRegion::FlightInformationRegion>> map;
    };

    TEST_F(ApiElementIteratorTest, ItReturnsTheCurrentValue)
    {
        FlowSdk::Api::ApiElementIterator<FlowSdk::FlightInformationRegion::FlightInformationRegion> iterator(
                mutex, map.cbegin()
        );

        const auto& iterVal = *iterator;
        EXPECT_EQ(fir1->Id(), iterVal.Id());
    }

    TEST_F(ApiElementIteratorTest, ItHasPointerOperator)
    {
        FlowSdk::Api::ApiElementIterator<FlowSdk::FlightInformationRegion::FlightInformationRegion> iterator(
                mutex, map.cbegin()
        );

        EXPECT_EQ(fir1->Id(), iterator->Id());
    }

    TEST_F(ApiElementIteratorTest, ItHasPostfixIncrement)
    {
        FlowSdk::Api::ApiElementIterator<FlowSdk::FlightInformationRegion::FlightInformationRegion> iterator(
                mutex, map.cbegin()
        );

        EXPECT_EQ(fir1->Id(), (iterator++)->Id());
        EXPECT_EQ(fir2->Id(), iterator->Id());
    }

    TEST_F(ApiElementIteratorTest, ItHasPrefixIncrement)
    {
        FlowSdk::Api::ApiElementIterator<FlowSdk::FlightInformationRegion::FlightInformationRegion> iterator(
                mutex, map.cbegin()
        );

        EXPECT_EQ(fir2->Id(), (++iterator)->Id());
        EXPECT_EQ(fir2->Id(), iterator->Id());
    }

    TEST_F(ApiElementIteratorTest, ItHasEquality)
    {
        std::recursive_mutex otherMutex;
        FlowSdk::Api::ApiElementIterator<FlowSdk::FlightInformationRegion::FlightInformationRegion> iterator(
                mutex, map.cbegin()
        );

        FlowSdk::Api::ApiElementIterator<FlowSdk::FlightInformationRegion::FlightInformationRegion> iterator2(
                otherMutex, map.cbegin()
        );

        EXPECT_TRUE(iterator == iterator2);
    }

    TEST_F(ApiElementIteratorTest, ItDoesntHaveEquality)
    {
        std::recursive_mutex otherMutex;
        FlowSdk::Api::ApiElementIterator<FlowSdk::FlightInformationRegion::FlightInformationRegion> iterator(
                mutex, map.cbegin()
        );

        FlowSdk::Api::ApiElementIterator<FlowSdk::FlightInformationRegion::FlightInformationRegion> iterator2(
                otherMutex, map.cbegin()
        );
        iterator++;

        EXPECT_FALSE(iterator == iterator2);
    }

    TEST_F(ApiElementIteratorTest, ItHasInequality)
    {
        std::recursive_mutex otherMutex;
        FlowSdk::Api::ApiElementIterator<FlowSdk::FlightInformationRegion::FlightInformationRegion> iterator(
                mutex, map.cbegin()
        );

        FlowSdk::Api::ApiElementIterator<FlowSdk::FlightInformationRegion::FlightInformationRegion> iterator2(
                otherMutex, map.cbegin()
        );
        iterator++;

        EXPECT_TRUE(iterator != iterator2);
    }

    TEST_F(ApiElementIteratorTest, ItDoesntHaveInequality)
    {
        std::recursive_mutex otherMutex;
        FlowSdk::Api::ApiElementIterator<FlowSdk::FlightInformationRegion::FlightInformationRegion> iterator(
                mutex, map.cbegin()
        );

        FlowSdk::Api::ApiElementIterator<FlowSdk::FlightInformationRegion::FlightInformationRegion> iterator2(
                otherMutex, map.cbegin()
        );

        EXPECT_FALSE(iterator != iterator2);
    }
}// namespace FlowSdkTest::Api
