#include "flowmeasure/ConcreteEventFilter.h"
#include "event/ConcreteEvent.h"
#include "flightinformationregion/ConcreteFlightInformationRegion.h"

namespace ECFMPTest::FlowMeasure {

    class ConcreteEventFilterTest : public testing::Test
    {
        public:
        ConcreteEventFilterTest()
            : fir(std::make_shared<ECFMP::FlightInformationRegion::ConcreteFlightInformationRegion>(1, "EGTT", "London")
            ),
              event(std::make_shared<ECFMP::Event::ConcreteEvent>(
                      1, "TEST", std::chrono::system_clock::now(), std::chrono::system_clock::now(), fir, "ABC"
              )),
              eventFilter(event, ECFMP::FlowMeasure::EventParticipation::NotParticipating)
        {}

        std::shared_ptr<ECFMP::FlightInformationRegion::ConcreteFlightInformationRegion> fir;
        std::shared_ptr<const ECFMP::Event::ConcreteEvent> event;
        ECFMP::FlowMeasure::ConcreteEventFilter eventFilter;
    };

    TEST_F(ConcreteEventFilterTest, ItReturnsEvents)
    {
        EXPECT_EQ(event.get(), &eventFilter.Event());
    }

    TEST_F(ConcreteEventFilterTest, ItReturnsParticipation)
    {
        EXPECT_EQ(ECFMP::FlowMeasure::EventParticipation::NotParticipating, eventFilter.Participation());
    }

    TEST_F(ConcreteEventFilterTest, ItIsNotParticipatingIn)
    {
        ECFMP::FlowMeasure::ConcreteEventFilter eventFilter2(
                event, ECFMP::FlowMeasure::EventParticipation::Participating
        );
        EXPECT_TRUE(eventFilter2.IsParticipating());
    }

    TEST_F(ConcreteEventFilterTest, ItHasEventApplicability)
    {
        ECFMP::Event::ConcreteEvent event2(
                2, "TEST", std::chrono::system_clock::now(), std::chrono::system_clock::now(), fir, "ABC"
        );
        EXPECT_TRUE(eventFilter.ApplicableToEvent(*event));
        EXPECT_FALSE(eventFilter.ApplicableToEvent(event2));
    }
}// namespace ECFMPTest::FlowMeasure
