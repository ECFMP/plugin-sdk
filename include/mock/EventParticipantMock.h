#pragma once
#include "../flow-sdk/EventParticipant.h"
#include <gmock/gmock.h>

namespace ECFMP::Mock::Event {

    class EventParticipantMock : public ECFMP::Event::EventParticipant
    {
        public:
        MOCK_METHOD(int, Cid, (), (const, noexcept, override));
        MOCK_METHOD(const std::string&, OriginAirport, (), (const, noexcept, override));
        MOCK_METHOD(const std::string&, DestinationAirport, (), (const, noexcept, override));
    };

}// namespace ECFMP::Mock::Event
