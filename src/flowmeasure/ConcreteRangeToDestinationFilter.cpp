#include "ConcreteRangeToDestinationFilter.h"
#include "euroscope/EuroscopeAircraft.h"

namespace ECFMP::FlowMeasure {
    ConcreteRangeToDestinationFilter::ConcreteRangeToDestinationFilter(int range) noexcept : range(range)
    {}

    auto ConcreteRangeToDestinationFilter::Range() const noexcept -> int
    {
        return range;
    }

    bool ConcreteRangeToDestinationFilter::ApplicableToAircraft(const Euroscope::EuroscopeAircraft& aircraft
    ) const noexcept
    {
        return ((int) ceil(aircraft.RangeToDestination())) <= range;
    }

    auto ConcreteRangeToDestinationFilter::FilterDescription() const noexcept -> std::string
    {
        return "Range to Destination Less Than: " + std::to_string(range) + "nm";
    }
}// namespace ECFMP::FlowMeasure
