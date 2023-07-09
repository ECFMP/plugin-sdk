#include "ConcreteFlightInformationRegion.h"

#include <utility>

namespace ECFMP::FlightInformationRegion {
    ConcreteFlightInformationRegion::ConcreteFlightInformationRegion(int id, std::string identifier, std::string name)
        : id(id), identifier(std::move(identifier)), name(std::move(name))
    {}

    auto ConcreteFlightInformationRegion::Id() const noexcept -> int
    {
        return id;
    }
    auto ConcreteFlightInformationRegion::Identifier() const noexcept -> const std::string&
    {
        return identifier;
    }
    auto ConcreteFlightInformationRegion::Name() const noexcept -> const std::string&
    {
        return name;
    }
}// namespace ECFMP::FlightInformationRegion
