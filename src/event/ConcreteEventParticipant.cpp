#include "ConcreteEventParticipant.h"

namespace FlowSdk::Event {
    ConcreteEventParticipant::ConcreteEventParticipant(int cid, std::string originAirfield,
                                                       std::string destinationAirfield)
        : cid(cid), originAirfield(std::move(originAirfield)), destinationAirfield(std::move(destinationAirfield))
    {}

    auto FlowSdk::Event::ConcreteEventParticipant::Cid() const noexcept -> int
    {
        return cid;
    }

    auto FlowSdk::Event::ConcreteEventParticipant::OriginAirport() const noexcept -> const std::string&
    {
        return originAirfield;
    }

    auto FlowSdk::Event::ConcreteEventParticipant::DestinationAirport() const noexcept -> const std::string&
    {
        return destinationAirfield;
    }
}// namespace FlowSdk::Event
