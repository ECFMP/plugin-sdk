#pragma once
#include "../flow-sdk/EventFilter.h"
#include <gmock/gmock.h>

namespace FlowSdk::Mock::FlowMeasure {

    class EventFilterMock : public FlowSdk::FlowMeasure::EventFilter
    {
        public:
        MOCK_METHOD(bool, ApplicableToEvent, (const FlowSdk::Event::Event&), (const, noexcept, override));
        MOCK_METHOD(const FlowSdk::Event::Event&, Event, (), (const, noexcept, override));
        MOCK_METHOD(bool, ParticipatingIn, (), (const, noexcept, override));
    };

}// namespace FlowSdk::Mock::FlowMeasure
