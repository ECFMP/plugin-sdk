#pragma once
#include "ECFMP/flowmeasure/RangeToDestinationFilter.h"

namespace ECFMP::FlowMeasure {

    class ConcreteRangeToDestinationFilter : public RangeToDestinationFilter
    {
        public:
        explicit ConcreteRangeToDestinationFilter(int range) noexcept;
        auto Range() const noexcept -> int override;
        auto ApplicableToAircraft(const Euroscope::EuroscopeAircraft& aircraft) const noexcept -> bool override;
        auto FilterDescription() const noexcept -> std::string override;

        private:
        // The range to destination.
        int range;
    };
}// namespace ECFMP::FlowMeasure
