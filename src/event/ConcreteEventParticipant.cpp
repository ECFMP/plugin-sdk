#include "ConcreteEventParticipant.h"

namespace FlowSdk::Event {
    ConcreteEventParticipant::ConcreteEventParticipant(int cid, const char* originAirfield,
                                                       const char* destinationAirfield)
        : cid(cid), originAirfield(originAirfield), destinationAirfield(destinationAirfield)
    {}

    int FlowSdk::Event::ConcreteEventParticipant::Cid() const
    {
        return cid;
    }

    const char* FlowSdk::Event::ConcreteEventParticipant::OriginAirfield() const
    {
        return originAirfield;
    }

    const char* FlowSdk::Event::ConcreteEventParticipant::DestinationAirfield() const
    {
        return destinationAirfield;
    }
}// namespace FlowSdk::Event
