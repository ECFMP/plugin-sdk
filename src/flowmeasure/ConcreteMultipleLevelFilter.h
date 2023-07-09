#pragma once
#include "flow-sdk/MultipleLevelFilter.h"

namespace ECFMP::FlowMeasure {

    class ConcreteMultipleLevelFilter : public MultipleLevelFilter
    {
        public:
        explicit ConcreteMultipleLevelFilter(std::vector<int> levels);

        /**
         * Returns the levels (e.g. 350) associated with this filter
         */
        [[nodiscard]] auto Levels() const noexcept -> std::vector<int> override;

        /**
         * Returns the altitudes (35000) associated with this filter
         */
        [[nodiscard]] auto Altitudes() const noexcept -> std::vector<int> override;

        /**
         * Helper method to determine, given a particular level (e.g. 350), does this filter apply.
         */
        [[nodiscard]] auto ApplicableToLevel(int level) const noexcept -> bool override;

        /**
         * Helper method to determine, given a particular altitude (e.g. 35000), does this filter apply.
         */
        [[nodiscard]] auto ApplicableToAltitude(int level) const noexcept -> bool override;

        private:
        // The levels (e.g. 350) associated with this filter
        std::vector<int> levels;

        // The altitudes (35000) associated with this filter
        std::vector<int> altitudes;
    };

}// namespace ECFMP::FlowMeasure
