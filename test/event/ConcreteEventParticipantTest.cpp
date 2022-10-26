#include "event/ConcreteEventParticipant.h"
#include "gtest/gtest.h"

using FlowSdk::Event::ConcreteEventParticipant;

namespace FlowSdkTest::Event {
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
        EXPECT_EQ(0, strcmp("EGKK", participant.OriginAirfield()));
    }

    TEST_F(ConcreteEventParticipantTest, ItHasADestination)
    {
        EXPECT_EQ(0, strcmp("EDDM", participant.DestinationAirfield()));
    }
}// namespace FlowSdkTest::Event
