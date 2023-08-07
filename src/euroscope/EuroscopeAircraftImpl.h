#pragma once
#include "EuroscopeAircraft.h"
#include <string>

namespace EuroScopePlugIn {
    class CFlightPlan;
    class CRadarTarget;
}// namespace EuroScopePlugIn

namespace ECFMP::Euroscope {

    class EuroscopeAircraftImpl : public EuroscopeAircraft
    {
        public:
        EuroscopeAircraftImpl(
                const EuroScopePlugIn::CFlightPlan& flightplan, const EuroScopePlugIn::CRadarTarget& radarTarget
        );
        [[nodiscard]] auto CurrentAltitudeStandardPressure() const -> int override;
        [[nodiscard]] auto CruiseAltitude() const -> int override;
        [[nodiscard]] auto DepartureAirport() const -> std::string override;
        [[nodiscard]] auto DestinationAirport() const -> std::string override;
        [[nodiscard]] auto Cid() const -> int override;
        [[nodiscard]] auto RangeToDestination() const -> double override;
        [[nodiscard]] auto GroundSpeed() const -> int override;
        [[nodiscard]] auto RouteString() const -> std::string override;

        private:
        // The underlying Euroscope objects, not owned by this class.
        const EuroScopePlugIn::CFlightPlan& flightplan;
        const EuroScopePlugIn::CRadarTarget& radarTarget;
    };
}// namespace ECFMP::Euroscope
