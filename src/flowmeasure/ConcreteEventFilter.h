#pragma once
#include "ECFMP/EventFilter.h"

namespace ECFMP::Event {
    class Event;
}// namespace ECFMP::Event

namespace ECFMP::FlowMeasure {

    class ConcreteEventFilter : public EventFilter
    {
        public:
        ConcreteEventFilter(
                const std::shared_ptr<const ECFMP::Event::Event>& event, EventParticipation participatingIn
        );
        [[nodiscard]] auto ApplicableToEvent(const ECFMP::Event::Event& applicableTo) const noexcept -> bool override;
        [[nodiscard]] auto Event() const noexcept -> const ECFMP::Event::Event& override;
        [[nodiscard]] auto Participation() const noexcept -> EventParticipation override;
        [[nodiscard]] auto IsParticipating() const noexcept -> bool override;

        private:
        // The event in question
        std::shared_ptr<const ECFMP::Event::Event> event;

        // Whether this is a participating in filter
        EventParticipation participatingIn;
    };

}// namespace ECFMP::FlowMeasure
