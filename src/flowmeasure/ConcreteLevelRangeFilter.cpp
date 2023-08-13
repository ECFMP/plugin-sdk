#include "ConcreteLevelRangeFilter.h"
#include "euroscope/EuroscopeAircraft.h"

namespace ECFMP::FlowMeasure {
    ConcreteLevelRangeFilter::ConcreteLevelRangeFilter(LevelRangeFilterType type, int filterLevel)
        : type(type), filterLevel(filterLevel), filterLevelAsAltitude(filterLevel * 100)
    {}

    auto ConcreteLevelRangeFilter::Type() const noexcept -> LevelRangeFilterType
    {
        return type;
    }

    auto ConcreteLevelRangeFilter::Level() const noexcept -> int
    {
        return filterLevel;
    }

    auto ConcreteLevelRangeFilter::Altitude() const noexcept -> int
    {
        return filterLevelAsAltitude;
    }

    auto ConcreteLevelRangeFilter::ApplicableToLevel(int level) const noexcept -> bool
    {
        return ApplicableToAltitude(level * 100);
    }

    auto ConcreteLevelRangeFilter::ApplicableToAltitude(int altitude) const noexcept -> bool
    {
        return type == LevelRangeFilterType::AtOrBelow ? altitude <= filterLevelAsAltitude
                                                       : altitude >= filterLevelAsAltitude;
    }

    bool ConcreteLevelRangeFilter::ApplicableToAircraft(const Euroscope::EuroscopeAircraft& aircraft) const noexcept
    {
        return ApplicableToAltitude(aircraft.CruiseAltitude());
    }

    auto ConcreteLevelRangeFilter::FilterDescription() const noexcept -> std::string
    {
        return type == LevelRangeFilterType::AtOrBelow ? "At or below: FL" + std::to_string(filterLevel)
                                                       : "At or above: FL" + std::to_string(filterLevel);
    }
}// namespace ECFMP::FlowMeasure
