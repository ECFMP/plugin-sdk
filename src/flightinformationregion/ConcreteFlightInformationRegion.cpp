#include "ConcreteFlightInformationRegion.h"

#include <utility>

namespace FlowSdk::FlightInformationRegion {
    ConcreteFlightInformationRegion::ConcreteFlightInformationRegion(int id, std::string identifier, std::string name)
        : id(id), identifier(std::move(identifier)), name(std::move(name))
    {}

    auto ConcreteFlightInformationRegion::Id() const -> int
    {
        return id;
    }
    auto ConcreteFlightInformationRegion::Identifier() const -> const std::string&
    {
        return identifier;
    }
    auto ConcreteFlightInformationRegion::Name() const -> const std::string&
    {
        return name;
    }
}// namespace FlowSdk::FlightInformationRegion
