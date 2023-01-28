#pragma once

namespace FlowSdk::FlowMeasure {
    /**
     * A filter that pertains to arrival or departure airports
     */
    class AirportFilter
    {
        public:
        virtual ~AirportFilter() = default;
        /**
         * Returns the raw airport strings that make up this filter.
         */
        [[nodiscard]] virtual auto AirportStrings() const noexcept -> const std::set<std::string>& = 0;

        /**
         * Helper method that checks, given an airport string, if the filter applies.
         */
        [[nodiscard]] virtual auto ApplicableToAirport(const std::string& airport) const noexcept -> bool = 0;
    };
}// namespace FlowSdk::FlowMeasure
