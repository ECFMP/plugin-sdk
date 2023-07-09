#pragma once
#include "../ECFMP/EventFilter.h"
#include <gmock/gmock.h>

namespace ECFMP::Mock::FlowMeasure {

    class EventFilterMock : public ECFMP::FlowMeasure::EventFilter
    {
        public:
        MOCK_METHOD(bool, ApplicableToEvent, (const ECFMP::Event::Event&), (const, noexcept, override));
        MOCK_METHOD(const ECFMP::Event::Event&, Event, (), (const, noexcept, override));
        MOCK_METHOD(ECFMP::FlowMeasure::EventParticipation, Participation, (), (const, noexcept, override));
        MOCK_METHOD(bool, IsParticipating, (), (const, noexcept, override));
    };

}// namespace ECFMP::Mock::FlowMeasure
