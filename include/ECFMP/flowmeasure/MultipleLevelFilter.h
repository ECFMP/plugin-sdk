#pragma once
#include "ChecksAircraftApplicability.h"

namespace ECFMP::FlowMeasure {
    /**
     * A filter that pertains to the cruising level of the aircraft.
     */
    class MultipleLevelFilter : public ChecksAircraftApplicability
    {
        public:
        virtual ~MultipleLevelFilter() = default;

        /**
         * Returns the levels (e.g. 350) associated with this filter
         */
        [[nodiscard]] virtual auto Levels() const noexcept -> std::vector<int> = 0;

        /**
         * Returns the altitudes (35000) associated with this filter
         */
        [[nodiscard]] virtual auto Altitudes() const noexcept -> std::vector<int> = 0;

        /**
         * Helper method to determine, given a particular level (e.g. 350), does this filter apply.
         */
        [[nodiscard]] virtual auto ApplicableToLevel(int level) const noexcept -> bool = 0;

        /**
         * Helper method to determine, given a particular altitude (e.g. 35000), does this filter apply.
         */
        [[nodiscard]] virtual auto ApplicableToAltitude(int level) const noexcept -> bool = 0;

        /**
         * Description of the filter
         */
        [[nodiscard]] virtual auto FilterDescription() const noexcept -> std::string = 0;
    };
}// namespace ECFMP::FlowMeasure
