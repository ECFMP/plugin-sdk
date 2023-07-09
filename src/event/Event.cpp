#include "ECFMP/Event.h"

namespace ECFMP::Event {
    auto Event::operator==(const class Event& event) const -> bool
    {
        return this->Id() == event.Id();
    }
}// namespace ECFMP::Event
