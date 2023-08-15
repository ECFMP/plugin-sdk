#include "ConcreteAirportFilter.h"
#include "euroscope/EuroscopeAircraft.h"

namespace ECFMP::FlowMeasure {
    ConcreteAirportFilter::ConcreteAirportFilter(std::set<std::string> airportStrings, AirportFilterType type) noexcept
        : airportStrings(std::move(airportStrings)), type(type)
    {}

    auto ConcreteAirportFilter::AirportStrings() const noexcept -> const std::set<std::string>&
    {
        return airportStrings;
    }

    /**
     * Take the supplied airport string and check that it matches at least one of the possibly
     * wildcarded airport strings.
     */
    auto ConcreteAirportFilter::ApplicableToAirport(const std::string& airport) const noexcept -> bool
    {
        return std::find_if(
                       airportStrings.begin(), airportStrings.end(),
                       [&airport](const std::string& airportString) -> bool {
                           const auto airportStringWithoutWildcards =
                                   airportString.substr(0, airportString.find(WILDCARD_CHAR));
                           return airportStringWithoutWildcards
                                   == airport.substr(0, airportStringWithoutWildcards.length());
                       }
               ) != airportStrings.cend();
    }

    auto ConcreteAirportFilter::Type() const noexcept -> AirportFilterType
    {
        return type;
    }

    bool ConcreteAirportFilter::ApplicableToAircraft(const Euroscope::EuroscopeAircraft& aircraft) const noexcept
    {
        return ApplicableToAirport(
                type == AirportFilterType::Departure ? aircraft.DepartureAirport() : aircraft.DestinationAirport()
        );
    }

    std::string ConcreteAirportFilter::FilterDescription() const noexcept
    {
        std::string description = type == AirportFilterType::Departure ? "Departing: " : "Arriving: ";

        for (const auto& airportString: airportStrings) {
            const auto isWildcard = airportString.find(WILDCARD_CHAR) != std::string::npos;
            description += isWildcard ? "Any of: " + airportString : airportString;
            description += ", ";
        }

        // Trim the trailing ", "
        return description.substr(0, description.length() - 2);
    }
}// namespace ECFMP::FlowMeasure
