#include "ConcreteEventParticipant.h"

namespace FlowSdk::Event {
    ConcreteEventParticipant::ConcreteEventParticipant(int cid, const char* originAirfield,
                                                       const char* destinationAirfield)
        : cid(cid), originAirfield(originAirfield), destinationAirfield(destinationAirfield)
    {}

    int FlowSdk::Event::ConcreteEventParticipant::Cid() const noexcept
    {
        return cid;
    }

    const char* FlowSdk::Event::ConcreteEventParticipant::OriginAirfield() const noexcept
    {
        return originAirfield;
    }

    const char* FlowSdk::Event::ConcreteEventParticipant::DestinationAirfield() const noexcept
    {
        return destinationAirfield;
    }
}// namespace FlowSdk::Event
