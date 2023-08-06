#pragma once
#include "EuroscopeAircraftFactory.h"

namespace ECFMP::Euroscope {
    class EuroscopeAircraftFactoryImpl : public EuroscopeAircraftFactory
    {
        public:
        [[nodiscard]] auto
        Make(const EuroScopePlugIn::CFlightPlan& flightplan, const EuroScopePlugIn::CRadarTarget& radarTarget
        ) const noexcept -> std::shared_ptr<EuroscopeAircraft> override;
    };
}// namespace ECFMP::Euroscope
