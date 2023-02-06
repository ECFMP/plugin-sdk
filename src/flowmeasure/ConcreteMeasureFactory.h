#pragma once
#include "ConcreteMeasure.h"

namespace FlowSdk::FlowMeasure {
    // Integer valued intervals
    [[nodiscard]] auto MinimumDepartureInterval(int interval) -> ConcreteMeasure;
    [[nodiscard]] auto AverageDepartureInterval(int interval) -> ConcreteMeasure;
    [[nodiscard]] auto PerHour(int perHour) -> ConcreteMeasure;
    [[nodiscard]] auto MilesInTrail(int milesInTrail) -> ConcreteMeasure;
    [[nodiscard]] auto MaxIndicatedAirspeed(int airspeed) -> ConcreteMeasure;
    [[nodiscard]] auto IndicatedAirspeedReduction(int airspeed) -> ConcreteMeasure;

    // Double valued intervals
    [[nodiscard]] auto MaxMach(double mach) -> ConcreteMeasure;
    [[nodiscard]] auto MachReduction(double mach) -> ConcreteMeasure;

    // Set valued intervals
    [[nodiscard]] auto MandatoryRoute(std::set<std::string> routes) -> ConcreteMeasure;

    // No-valued intervals
    [[nodiscard]] auto Prohibit() -> ConcreteMeasure;
    [[nodiscard]] auto GroundStop() -> ConcreteMeasure;
}// namespace FlowSdk::FlowMeasure
