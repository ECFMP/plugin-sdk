#pragma once

namespace FlowSdk::FlightInformationRegion {

    /**
     * A flight information region.
     */
    class FlightInformationRegion
    {
        public:
        virtual ~FlightInformationRegion() = default;

        /**
         * The id on the API.
         */
        [[nodiscard]] virtual auto Id() const noexcept -> int = 0;

        /**
         * The identifier of the FIR, for example "EGTT".
         */
        [[nodiscard]] virtual auto Identifier() const noexcept -> const char* = 0;

        /**
         * The name of the FIR, for example "London"
         */
        [[nodiscard]] virtual auto Name() const noexcept -> const char* = 0;
    };
}// namespace FlowSdk::FlightInformationRegion
