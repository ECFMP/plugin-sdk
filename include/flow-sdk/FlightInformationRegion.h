#pragma once
#include "ApiElement.h"
#include "flow-sdk/ApiElementCollection.h"

namespace ECFMP::FlightInformationRegion {

    /**
     * A flight information region.
     */
    class FlightInformationRegion : public Api::StringIdentifiableApiElement
    {
        public:
        virtual ~FlightInformationRegion() = default;

        /**
         * The id on the API.
         */
        [[nodiscard]] auto Id() const noexcept -> int override = 0;

        /**
         * The identifier of the FIR, for example "EGTT".
         */
        [[nodiscard]] auto Identifier() const noexcept -> const std::string& override = 0;

        /**
         * The name of the FIR, for example "London"
         */
        [[nodiscard]] virtual auto Name() const noexcept -> const std::string& = 0;
    };
}// namespace ECFMP::FlightInformationRegion
