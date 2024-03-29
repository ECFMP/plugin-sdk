#pragma once
#include "ChecksAircraftApplicability.h"
#include "LevelRangeFilterType.h"

namespace ECFMP::FlowMeasure {

    /**
     * A filter that pertains to the cruising level of the aircraft.
     */
    class LevelRangeFilter : public ChecksAircraftApplicability
    {
        public:
        virtual ~LevelRangeFilter() = default;

        /**
         * Returns the type of this level filter.
         */
        [[nodiscard]] virtual auto Type() const noexcept -> LevelRangeFilterType = 0;

        /**
         * Returns the level (e.g. 350) associated with this filter
         */
        [[nodiscard]] virtual auto Level() const noexcept -> int = 0;

        /**
         * Returns the altitude (35000) associated with this filter
         */
        [[nodiscard]] virtual auto Altitude() const noexcept -> int = 0;

        /**
         * Helper method to determine, given a particular level (e.g. 350), does this filter apply.
         */
        [[nodiscard]] virtual auto ApplicableToLevel(int level) const noexcept -> bool = 0;

        /**
         * Helper method to determine, given a particular altitude (e.g. 35000), does this filter apply.
         */
        [[nodiscard]] virtual auto ApplicableToAltitude(int level) const noexcept -> bool = 0;

        /**
         * Description of the filter.
         */
        [[nodiscard]] virtual auto FilterDescription() const noexcept -> std::string = 0;
    };
}// namespace ECFMP::FlowMeasure
