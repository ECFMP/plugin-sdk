#include "EuroscopeAircraftImpl.h"
#include "EuroScopePlugIn.h"

namespace ECFMP::Euroscope {
    EuroscopeAircraftImpl::EuroscopeAircraftImpl(
            const EuroScopePlugIn::CFlightPlan& flightplan, const EuroScopePlugIn::CRadarTarget& radarTarget
    )
        : flightplan(flightplan), radarTarget(radarTarget)
    {}

    auto EuroscopeAircraftImpl::CurrentAltitudeStandardPressure() const -> int
    {
        return flightplan.GetFPTrackPosition().GetFlightLevel();
    }

    auto EuroscopeAircraftImpl::CruiseAltitude() const -> int
    {
        return flightplan.GetFlightPlanData().GetFinalAltitude();
    }

    auto EuroscopeAircraftImpl::DepartureAirport() const -> std::string
    {
        return flightplan.GetFlightPlanData().GetOrigin();
    }

    auto EuroscopeAircraftImpl::DestinationAirport() const -> std::string
    {
        return flightplan.GetFlightPlanData().GetDestination();
    }

    auto EuroscopeAircraftImpl::Cid() const -> int
    {
        // TODO: Implement this, EuroScope doesn't have a CID property that we can use
        return 0;
    }

    auto EuroscopeAircraftImpl::RangeToDestination() const -> double
    {
        return flightplan.GetDistanceToDestination();
    }

    auto EuroscopeAircraftImpl::GroundSpeed() const -> int
    {
        return radarTarget.GetGS();
    }

    auto EuroscopeAircraftImpl::RouteString() const -> std::string
    {
        return flightplan.GetFlightPlanData().GetRoute();
    }
}// namespace ECFMP::Euroscope
