#pragma once
#include "ECFMP/event/Event.h"
#include <gmock/gmock.h>

namespace ECFMP::Mock::Event {

    class EventMock : public ECFMP::Event::Event
    {
        public:
        MOCK_METHOD(int, Id, (), (const, noexcept, override));
        MOCK_METHOD(const std::string&, Name, (), (const, noexcept, override));
        MOCK_METHOD(const std::chrono::system_clock::time_point&, Start, (), (const, noexcept, override));
        MOCK_METHOD(const std::chrono::system_clock::time_point&, End, (), (const, noexcept, override));
        MOCK_METHOD(
                const ECFMP::FlightInformationRegion::FlightInformationRegion&, FlightInformationRegion, (),
                (const, noexcept, override)
        );
        MOCK_METHOD(const std::string&, VatcanCode, (), (const, noexcept, override));
    };

}// namespace ECFMP::Mock::Event
