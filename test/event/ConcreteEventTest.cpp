#include "event/ConcreteEvent.h"
#include "flightinformationregion/ConcreteFlightInformationRegion.h"

using FlowSdk::Event::ConcreteEvent;
using FlowSdk::FlightInformationRegion::ConcreteFlightInformationRegion;

namespace FlowSdkTest::Event {
    class ConcreteEventTest : public testing::Test
    {
        public:
        ConcreteEventTest()
            : start(std::chrono::time_point<std::chrono::system_clock>() + std::chrono::seconds(1666803606)),
              end(std::chrono::time_point<std::chrono::system_clock>() + std::chrono::seconds(1666803626)),
              fir(std::make_shared<ConcreteFlightInformationRegion>(1, "EGTT", "London")),
              event(1, "Some event", start, end, fir, "VATCAN")
        {}

        // 1666803606
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
        EXPECT_EQ(0, strcmp("Some event", event.Name()));
    }

    TEST_F(ConcreteEventTest, ItHasAStart)
    {
        EXPECT_EQ(1666803606, event.Start());
    }

    TEST_F(ConcreteEventTest, ItHasAEnd)
    {
        EXPECT_EQ(1666803626, event.End());
    }

    TEST_F(ConcreteEventTest, ItHasAFir)
    {
        EXPECT_EQ(fir.get(), &event.FlightInformationRegion());
    }

    TEST_F(ConcreteEventTest, ItHasAVatcanCode)
    {
        EXPECT_EQ(0, strcmp("VATCAN", event.VatcanCode()));
    }
}// namespace FlowSdkTest::Event
