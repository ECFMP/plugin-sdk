#pragma once

namespace ECFMP::FlowMeasure {
    /**
     * A filter that pertains to arrival or departure airports
     */
    class RangeToDestinationFilter
    {
        public:
        virtual ~RangeToDestinationFilter() = default;

        /**
         * Returns the range to destination.
         */
        [[nodiscard]] virtual auto Range() const noexcept -> int = 0;
    };
}// namespace ECFMP::FlowMeasure
