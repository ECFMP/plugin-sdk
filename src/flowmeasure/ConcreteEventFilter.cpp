#include "ConcreteEventFilter.h"
#include "flow-sdk/Event.h"

namespace FlowSdk::FlowMeasure {

    ConcreteEventFilter::ConcreteEventFilter(const std::shared_ptr<const FlowSdk::Event::Event>& event,
                                             bool participatingIn)
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

    auto ConcreteEventFilter::ParticipatingIn() const noexcept -> bool
    {
        return participatingIn;
    }
}// namespace FlowSdk::FlowMeasure
