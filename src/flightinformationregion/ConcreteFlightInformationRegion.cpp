#include "ConcreteFlightInformationRegion.h"

namespace FlowSdk::FlightInformationRegion {
    ConcreteFlightInformationRegion::ConcreteFlightInformationRegion(int id, const char* identifier, const char* name)
        : id(id), identifier(identifier), name(name)
    {}

    auto ConcreteFlightInformationRegion::Id() const noexcept -> int
    {
        return id;
    }
    auto ConcreteFlightInformationRegion::Identifier() const noexcept -> const char*
    {
        return identifier;
    }
    auto ConcreteFlightInformationRegion::Name() const noexcept -> const char*
    {
        return name;
    }
}// namespace FlowSdk::FlightInformationRegion
