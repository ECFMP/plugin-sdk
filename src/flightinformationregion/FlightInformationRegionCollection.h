#pragma once

namespace FlowSdk::FlightInformationRegion {

    class FlightInformationRegion;

    /**
     * A collection of all the flight information regions.
     */
    class FlightInformationRegionCollection
    {
        public:
        virtual ~FlightInformationRegionCollection() = default;

        /**
         * Returns the FIR, by the internal FIR id.
         */
        [[nodiscard]] virtual auto Get(int id) const -> std::shared_ptr<class FlightInformationRegion> = 0;

        /**
         * Returns the FIR, by the identifier (e.g. EGTT).
         */
        [[nodiscard]] virtual auto Get(const std::string& identifier) const
                -> std::shared_ptr<class FlightInformationRegion> = 0;
    };
}// namespace FlowSdk::FlightInformationRegion
