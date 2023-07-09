#include "ConcreteMeasureFactory.h"

#include <utility>

namespace ECFMP::FlowMeasure {

    [[nodiscard]] auto IntegerMeasure(MeasureType type, int value) -> ConcreteMeasure
    {
        return {type, value};
    }

    [[nodiscard]] auto DoubleMeasure(MeasureType type, double value) -> ConcreteMeasure
    {
        return {type, value};
    }

    [[nodiscard]] auto SetMeasure(MeasureType type, std::set<std::string> value) -> ConcreteMeasure
    {
        return {type, std::move(value)};
    }

    [[nodiscard]] auto VoidMeasure(MeasureType type) -> ConcreteMeasure
    {
        return ConcreteMeasure(type);
    }

    auto MinimumDepartureInterval(int interval) -> ConcreteMeasure
    {
        return IntegerMeasure(MeasureType::MinimumDepartureInterval, interval);
    }

    auto AverageDepartureInterval(int interval) -> ConcreteMeasure
    {
        return IntegerMeasure(MeasureType::AverageDepartureInterval, interval);
    }

    auto PerHour(int perHour) -> ConcreteMeasure
    {
        return IntegerMeasure(MeasureType::PerHour, perHour);
    }

    auto MilesInTrail(int milesInTrail) -> ConcreteMeasure
    {
        return IntegerMeasure(MeasureType::MilesInTrail, milesInTrail);
    }

    auto MaxIndicatedAirspeed(int airspeed) -> ConcreteMeasure
    {
        return IntegerMeasure(MeasureType::MaxIndicatedAirspeed, airspeed);
    }

    auto IndicatedAirspeedReduction(int airspeed) -> ConcreteMeasure
    {
        return IntegerMeasure(MeasureType::IndicatedAirspeedReduction, airspeed);
    }

    auto MaxMach(double mach) -> ConcreteMeasure
    {
        return DoubleMeasure(MeasureType::MaxMach, mach);
    }

    auto MachReduction(double mach) -> ConcreteMeasure
    {
        return DoubleMeasure(MeasureType::MachReduction, mach);
    }

    auto MandatoryRoute(std::set<std::string> routes) -> ConcreteMeasure
    {
        return SetMeasure(MeasureType::MandatoryRoute, std::move(routes));
    }

    auto Prohibit() -> ConcreteMeasure
    {
        return VoidMeasure(MeasureType::Prohibit);
    }

    auto GroundStop() -> ConcreteMeasure
    {
        return VoidMeasure(MeasureType::GroundStop);
    }
}// namespace ECFMP::FlowMeasure
