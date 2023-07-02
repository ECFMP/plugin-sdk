#pragma once
#include "flow-sdk/AirportFilter.h"

namespace FlowSdk::FlowMeasure {

    // TODO: ADES vs ADEP
    class ConcreteAirportFilter : public AirportFilter
    {
        public:
        explicit ConcreteAirportFilter(std::set<std::string> airportStrings);
        auto AirportStrings() const noexcept -> const std::set<std::string>& override;
        auto ApplicableToAirport(const std::string& airport) const noexcept -> bool override;

        private:
        // The strings for the airport filter, may contain wildcards (*)
        std::set<std::string> airportStrings;

        // The character used to denote wildcards
        static const char WILDCARD_CHAR = '*';
    };

}// namespace FlowSdk::FlowMeasure
