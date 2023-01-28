#include "ConcreteLevelFilter.h"

namespace FlowSdk::FlowMeasure {
    ConcreteLevelFilter::ConcreteLevelFilter(LevelFilterType type, int filterLevel)
        : type(type), filterLevel(filterLevel), filterLevelAsAltitude(filterLevel * 100)
    {}

    auto ConcreteLevelFilter::Type() const noexcept -> LevelFilterType
    {
        return type;
    }

    auto ConcreteLevelFilter::Level() const noexcept -> int
    {
        return filterLevel;
    }

    auto ConcreteLevelFilter::Altitude() const noexcept -> int
    {
        return filterLevelAsAltitude;
    }

    auto ConcreteLevelFilter::ApplicableToLevel(int level) const noexcept -> bool
    {
        return ApplicableToAltitude(level * 100);
    }

    auto ConcreteLevelFilter::ApplicableToAltitude(int altitude) const noexcept -> bool
    {
        if (type == LevelFilterType::At) {
            return altitude == filterLevelAsAltitude;
        }

        return type == LevelFilterType::AtOrBelow ? altitude <= filterLevelAsAltitude
                                                  : altitude >= filterLevelAsAltitude;
    }
}// namespace FlowSdk::FlowMeasure
