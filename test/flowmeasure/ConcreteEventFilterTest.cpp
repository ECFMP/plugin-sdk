#include "flowmeasure/ConcreteEventFilter.h"
#include "event/ConcreteEvent.h"
#include "flightinformationregion/ConcreteFlightInformationRegion.h"

namespace FlowSdkTest::FlowMeasure {

    class ConcreteEventFilterTest : public testing::Test
    {
        public:
        ConcreteEventFilterTest()
            : fir(std::make_shared<FlowSdk::FlightInformationRegion::ConcreteFlightInformationRegion>(1, "EGTT",
                                                                                                      "London")),
              event(std::make_shared<FlowSdk::Event::ConcreteEvent>(1, "TEST", std::chrono::system_clock::now(),
                                                                    std::chrono::system_clock::now(), fir, "ABC")),
              eventFilter(event, false)
        {}

        std::shared_ptr<FlowSdk::FlightInformationRegion::ConcreteFlightInformationRegion> fir;
        std::shared_ptr<const FlowSdk::Event::ConcreteEvent> event;
        FlowSdk::FlowMeasure::ConcreteEventFilter eventFilter;
    };

    TEST_F(ConcreteEventFilterTest, ItReturnsEvents)
    {
        EXPECT_EQ(event.get(), &eventFilter.Event());
    }

    TEST_F(ConcreteEventFilterTest, ItReturnsParticipation)
    {
        EXPECT_FALSE(eventFilter.ParticipatingIn());
    }

    TEST_F(ConcreteEventFilterTest, ItHasEventApplicability)
    {
        FlowSdk::Event::ConcreteEvent event2(2, "TEST", std::chrono::system_clock::now(),
                                             std::chrono::system_clock::now(), fir, "ABC");
        EXPECT_TRUE(eventFilter.ApplicableToEvent(*event));
        EXPECT_FALSE(eventFilter.ApplicableToEvent(event2));
    }
}// namespace FlowSdkTest::FlowMeasure
