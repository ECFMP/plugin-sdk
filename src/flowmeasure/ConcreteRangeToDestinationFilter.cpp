#include "ConcreteRangeToDestinationFilter.h"

namespace ECFMP::FlowMeasure {
    ConcreteRangeToDestinationFilter::ConcreteRangeToDestinationFilter(int range) noexcept : range(range)
    {}

    auto ConcreteRangeToDestinationFilter::Range() const noexcept -> int
    {
        return range;
    }
}// namespace ECFMP::FlowMeasure
