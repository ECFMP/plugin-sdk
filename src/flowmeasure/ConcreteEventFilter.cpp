#include "ConcreteEventFilter.h"
#include "flow-sdk/Event.h"

namespace FlowSdk::FlowMeasure {

    ConcreteEventFilter::ConcreteEventFilter(const std::shared_ptr<const FlowSdk::Event::Event>& event,
                                             EventParticipation participatingIn)
        : event(event), participatingIn(participatingIn)
    {
        assert(event && "Event not set in event filter");
    }

    auto ConcreteEventFilter::ApplicableToEvent(const FlowSdk::Event::Event& applicableTo) const noexcept -> bool
    {
        return applicableTo == *this->event;
    }

    auto ConcreteEventFilter::Event() const noexcept -> const FlowSdk::Event::Event&
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
}// namespace FlowSdk::FlowMeasure
