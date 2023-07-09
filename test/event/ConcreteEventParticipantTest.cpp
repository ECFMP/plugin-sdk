#include "event/ConcreteEventParticipant.h"

using ECFMP::Event::ConcreteEventParticipant;

namespace ECFMPTest::Event {
    class ConcreteEventParticipantTest : public testing::Test
    {
        public:
        ConcreteEventParticipantTest() : participant(12345, "EGKK", "EDDM")
        {}

        ConcreteEventParticipant participant;
    };

    TEST_F(ConcreteEventParticipantTest, ItHasACid)
    {
        EXPECT_EQ(12345, participant.Cid());
    }

    TEST_F(ConcreteEventParticipantTest, ItHasAnOrigin)
    {
        EXPECT_EQ("EGKK", participant.OriginAirport());
    }

    TEST_F(ConcreteEventParticipantTest, ItHasADestination)
    {
        EXPECT_EQ("EDDM", participant.DestinationAirport());
    }
}// namespace ECFMPTest::Event
