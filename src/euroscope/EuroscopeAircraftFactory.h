#pragma once

namespace EuroScopePlugIn {
    class CFlightPlan;
    class CRadarTarget;
}// namespace EuroScopePlugIn

namespace ECFMP::Euroscope {

    class EuroscopeAircraft;

    /**
     * A factory class for EuroscopeAircraft objects.
     */
    class EuroscopeAircraftFactory
    {
        public:
        virtual ~EuroscopeAircraftFactory() = default;

        [[nodiscard]] virtual auto
        Make(const EuroScopePlugIn::CFlightPlan& flightplan,
             const EuroScopePlugIn::CRadarTarget& radarTarget) const noexcept -> std::shared_ptr<EuroscopeAircraft> = 0;
    };
}// namespace ECFMP::Euroscope
