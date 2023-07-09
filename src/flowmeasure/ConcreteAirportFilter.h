#pragma once
#include "ECFMP/AirportFilter.h"

namespace ECFMP::FlowMeasure {

    class ConcreteAirportFilter : public AirportFilter
    {
        public:
        ConcreteAirportFilter(std::set<std::string> airportStrings, AirportFilterType type) noexcept;
        [[nodiscard]] auto AirportStrings() const noexcept -> const std::set<std::string>& override;
        [[nodiscard]] auto ApplicableToAirport(const std::string& airport) const noexcept -> bool override;
        [[nodiscard]] auto Type() const noexcept -> AirportFilterType override;

        private:
        // The strings for the airport filter, may contain wildcards (*)
        std::set<std::string> airportStrings;

        // The character used to denote wildcards
        static const char WILDCARD_CHAR = '*';

        // The type of airport filter
        AirportFilterType type;
    };

}// namespace ECFMP::FlowMeasure
