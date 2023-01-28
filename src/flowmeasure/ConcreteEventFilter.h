#pragma once
#include "EventFilter.h"

namespace FlowSdk::Event {
    class Event;
}// namespace FlowSdk::Event

namespace FlowSdk::FlowMeasure {

    class ConcreteEventFilter : public EventFilter
    {
        public:
        ConcreteEventFilter(const std::shared_ptr<const FlowSdk::Event::Event>& event, bool participatingIn);
        [[nodiscard]] auto ApplicableToEvent(const FlowSdk::Event::Event& applicableTo) const noexcept -> bool override;
        [[nodiscard]] auto Event() const noexcept -> const FlowSdk::Event::Event& override;
        [[nodiscard]] auto ParticipatingIn() const noexcept -> bool override;

        private:
        // The event in question
        std::shared_ptr<const FlowSdk::Event::Event> event;

        // Whether this is a participating in filter
        bool participatingIn;
    };

}// namespace FlowSdk::FlowMeasure
