#pragma once

namespace ECFMP::Euroscope {
    class EuroscopeAircraft;
}// namespace ECFMP::Euroscope

namespace ECFMP::FlowMeasure {
    class ChecksAircraftApplicability
    {
        public:
        virtual ~ChecksAircraftApplicability() = default;

        /**
         * Returns whether the given aircraft is applicable to this filter.
         */
        [[nodiscard]] virtual auto ApplicableToAircraft(const Euroscope::EuroscopeAircraft& aircraft) const noexcept
                -> bool = 0;
    };
}// namespace ECFMP::FlowMeasure
