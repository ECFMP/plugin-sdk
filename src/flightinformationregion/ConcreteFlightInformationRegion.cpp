#include "ConcreteFlightInformationRegion.h"

namespace FlowSdk::FlightInformationRegion {
    ConcreteFlightInformationRegion::ConcreteFlightInformationRegion(int id, const char* identifier, const char* name)
        : id(id), identifier(identifier), name(name)
    {}

    auto ConcreteFlightInformationRegion::Id() const -> int
    {
        return id;
    }
    auto ConcreteFlightInformationRegion::Identifier() const -> const char*
    {
        return identifier;
    }
    auto ConcreteFlightInformationRegion::Name() const -> const char*
    {
        return name;
    }
}// namespace FlowSdk::FlightInformationRegion
