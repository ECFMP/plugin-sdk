#pragma once
#include "../flow-sdk/Event.h"
#include <gmock/gmock.h>

namespace FlowSdk::Mock::Event {

    class EventMock : public FlowSdk::Event::Event
    {
        public:
        MOCK_METHOD(int, Id, (), (const, noexcept, override));
        MOCK_METHOD(const std::string&, Name, (), (const, noexcept, override));
        MOCK_METHOD(const std::chrono::system_clock::time_point&, Start, (), (const, noexcept, override));
        MOCK_METHOD(const std::chrono::system_clock::time_point&, End, (), (const, noexcept, override));
        MOCK_METHOD(const FlowSdk::FlightInformationRegion::FlightInformationRegion&, FlightInformationRegion, (),
                    (const, noexcept, override));
        MOCK_METHOD(const std::string&, VatcanCode, (), (const, noexcept, override));
    };

}// namespace FlowSdk::Mock::Event
