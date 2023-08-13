#pragma once
#include "ChecksAircraftApplicability.h"

namespace ECFMP::FlowMeasure {
    /**
     * A filter that pertains to arrival or departure airports
     */
    class RangeToDestinationFilter : public ChecksAircraftApplicability
    {
        public:
        ~RangeToDestinationFilter() override = default;

        /**
         * Returns the range to destination.
         */
        [[nodiscard]] virtual auto Range() const noexcept -> int = 0;

        /**
         * Description of the filter
         */
        [[nodiscard]] virtual auto FilterDescription() const noexcept -> std::string = 0;
    };
}// namespace ECFMP::FlowMeasure
