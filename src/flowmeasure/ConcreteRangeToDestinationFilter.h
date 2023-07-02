#pragma once
#include "flow-sdk/RangeToDestinationFilter.h"

namespace FlowSdk::FlowMeasure {

    class ConcreteRangeToDestinationFilter : public RangeToDestinationFilter
    {
        public:
        explicit ConcreteRangeToDestinationFilter(int range) noexcept;
        auto Range() const noexcept -> int override;

        private:
        // The range to destination.
        int range;
    };
}// namespace FlowSdk::FlowMeasure
