#include "ConcreteRangeToDestinationFilter.h"

namespace FlowSdk::FlowMeasure {
    ConcreteRangeToDestinationFilter::ConcreteRangeToDestinationFilter(int range) noexcept : range(range)
    {}

    auto ConcreteRangeToDestinationFilter::Range() const noexcept -> int
    {
        return range;
    }
}// namespace FlowSdk::FlowMeasure
