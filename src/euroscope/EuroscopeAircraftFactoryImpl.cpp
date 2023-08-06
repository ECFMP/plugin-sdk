#include "EuroscopeAircraftFactoryImpl.h"
#include "EuroscopeAircraftImpl.h"

namespace ECFMP::Euroscope {

    auto EuroscopeAircraftFactoryImpl::Make(
            const EuroScopePlugIn::CFlightPlan& flightplan, const EuroScopePlugIn::CRadarTarget& radarTarget
    ) const noexcept -> std::shared_ptr<EuroscopeAircraft>
    {
        return std::make_shared<EuroscopeAircraftImpl>(flightplan, radarTarget);
    }
}// namespace ECFMP::Euroscope
