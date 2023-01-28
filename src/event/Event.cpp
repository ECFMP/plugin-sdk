#include "flow-sdk/Event.h"

namespace FlowSdk::Event {
    auto Event::operator==(const class Event& event) const -> bool
    {
        return this->Id() == event.Id();
    }
}// namespace FlowSdk::Event
