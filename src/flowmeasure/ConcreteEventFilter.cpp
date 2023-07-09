#include "ConcreteEventFilter.h"
#include "ECFMP/event/Event.h"

namespace ECFMP::FlowMeasure {

    ConcreteEventFilter::ConcreteEventFilter(
            const std::shared_ptr<const ECFMP::Event::Event>& event, EventParticipation participatingIn
    )
        : event(event), participatingIn(participatingIn)
    {
        assert(event && "Event not set in event filter");
    }

    auto ConcreteEventFilter::ApplicableToEvent(const ECFMP::Event::Event& applicableTo) const noexcept -> bool
    {
        return applicableTo == *this->event;
    }

    auto ConcreteEventFilter::Event() const noexcept -> const ECFMP::Event::Event&
    {
        return *this->event;
    }

    auto ConcreteEventFilter::Participation() const noexcept -> EventParticipation
    {
        return participatingIn;
    }

    auto ConcreteEventFilter::IsParticipating() const noexcept -> bool
    {
        return Participation() == EventParticipation::Participating;
    }
}// namespace ECFMP::FlowMeasure
