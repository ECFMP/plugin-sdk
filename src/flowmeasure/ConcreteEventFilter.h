#pragma once
#include "flow-sdk/EventFilter.h"

namespace FlowSdk::Event {
    class Event;
}// namespace FlowSdk::Event

namespace FlowSdk::FlowMeasure {

    class ConcreteEventFilter : public EventFilter
    {
        public:
        ConcreteEventFilter(const std::shared_ptr<const FlowSdk::Event::Event>& event,
                            EventParticipation participatingIn);
        [[nodiscard]] auto ApplicableToEvent(const FlowSdk::Event::Event& applicableTo) const noexcept -> bool override;
        [[nodiscard]] auto Event() const noexcept -> const FlowSdk::Event::Event& override;
        [[nodiscard]] auto Participation() const noexcept -> EventParticipation override;
        [[nodiscard]] auto IsParticipating() const noexcept -> bool override;

        private:
        // The event in question
        std::shared_ptr<const FlowSdk::Event::Event> event;

        // Whether this is a participating in filter
        EventParticipation participatingIn;
    };

}// namespace FlowSdk::FlowMeasure
