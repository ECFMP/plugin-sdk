#include "ConcreteEventParticipant.h"

namespace ECFMP::Event {
    ConcreteEventParticipant::ConcreteEventParticipant(
            int cid, std::string originAirfield, std::string destinationAirfield
    )
        : cid(cid), originAirfield(std::move(originAirfield)), destinationAirfield(std::move(destinationAirfield))
    {}

    auto ECFMP::Event::ConcreteEventParticipant::Cid() const noexcept -> int
    {
        return cid;
    }

    auto ECFMP::Event::ConcreteEventParticipant::OriginAirport() const noexcept -> const std::string&
    {
        return originAirfield;
    }

    auto ECFMP::Event::ConcreteEventParticipant::DestinationAirport() const noexcept -> const std::string&
    {
        return destinationAirfield;
    }
}// namespace ECFMP::Event
