#pragma once
#include "../flow-sdk/EventParticipant.h"
#include <gmock/gmock.h>

namespace FlowSdk::Mock::Event {

    class EventParticipantMock : public FlowSdk::Event::EventParticipant
    {
        public:
        MOCK_METHOD(int, Cid, (), (const, noexcept, override));
        MOCK_METHOD(const std::string&, OriginAirport, (), (const, noexcept, override));
        MOCK_METHOD(const std::string&, DestinationAirport, (), (const, noexcept, override));
    };

}// namespace FlowSdk::Mock::Event
