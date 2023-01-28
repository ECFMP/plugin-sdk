#pragma once
#include "flow-sdk/LevelFilter.h"

namespace FlowSdk::FlowMeasure {

    class ConcreteLevelFilter : public LevelFilter
    {
        public:
        ConcreteLevelFilter(LevelFilterType type, int filterLevel);
        auto Type() const noexcept -> LevelFilterType override;
        auto Level() const noexcept -> int override;
        auto Altitude() const noexcept -> int override;
        auto ApplicableToLevel(int level) const noexcept -> bool override;
        auto ApplicableToAltitude(int level) const noexcept -> bool override;

        private:
        // The type of level filter this is
        LevelFilterType type;

        // The flight level to use for the filter
        int filterLevel;

        // The flight level for the filter, as an altitude
        int filterLevelAsAltitude;
    };

}// namespace FlowSdk::FlowMeasure
