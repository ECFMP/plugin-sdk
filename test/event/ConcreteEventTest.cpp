#include "event/ConcreteEvent.h"
#include "flightinformationregion/ConcreteFlightInformationRegion.h"
#include "gtest/gtest.h"

using FlowSdk::Event::ConcreteEvent;
using FlowSdk::FlightInformationRegion::ConcreteFlightInformationRegion;

namespace FlowSdkTest::Event {
    class ConcreteEventTest : public testing::Test
    {
        public:
        ConcreteEventTest()
            : start(std::chrono::system_clock::now()), end(std::chrono::system_clock::now() + std::chrono::minutes(10)),
              fir(std::make_shared<ConcreteFlightInformationRegion>(1, "EGTT", "London")),
              event(1, "Some event", start, end, fir)
        {}

        std::chrono::system_clock::time_point start;
        std::chrono::system_clock::time_point end;
        std::shared_ptr<ConcreteFlightInformationRegion> fir;
        ConcreteEvent event;
    };

    TEST_F(ConcreteEventTest, ItHasAnId)
    {
        EXPECT_EQ(1, event.Id());
    }

    TEST_F(ConcreteEventTest, ItHasAName)
    {
        EXPECT_EQ("Some event", event.Name());
    }

    TEST_F(ConcreteEventTest, ItHasAStart)
    {
        EXPECT_EQ(start, event.Start());
    }

    TEST_F(ConcreteEventTest, ItHasAEnd)
    {
        EXPECT_EQ(end, event.End());
    }

    TEST_F(ConcreteEventTest, ItHasAFir)
    {
        EXPECT_EQ(fir.get(), &event.FlightInformationRegion());
    }
}// namespace FlowSdkTest::Event