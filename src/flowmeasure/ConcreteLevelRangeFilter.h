#pragma once
#include "flow-sdk/LevelRangeFilter.h"

namespace FlowSdk::FlowMeasure {

    class ConcreteLevelRangeFilter : public LevelRangeFilter
    {
        public:
        ConcreteLevelRangeFilter(LevelRangeFilterType type, int filterLevel);
        [[nodiscard]] auto Type() const noexcept -> LevelRangeFilterType override;
        [[nodiscard]] auto Level() const noexcept -> int override;
        [[nodiscard]] auto Altitude() const noexcept -> int override;
        [[nodiscard]] auto ApplicableToLevel(int level) const noexcept -> bool override;
        [[nodiscard]] auto ApplicableToAltitude(int level) const noexcept -> bool override;

        private:
        // The type of level filter this is
        LevelRangeFilterType type;

        // The flight level to use for the filter
        int filterLevel;

        // The flight level for the filter, as an altitude
        int filterLevelAsAltitude;
    };

}// namespace FlowSdk::FlowMeasure
