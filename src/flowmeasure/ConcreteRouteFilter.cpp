#include "ConcreteRouteFilter.h"
#include "euroscope/EuroscopeAircraft.h"

namespace ECFMP::FlowMeasure {
    ConcreteRouteFilter::ConcreteRouteFilter(std::set<std::string> routes) : routes(std::move(routes))
    {}

    auto ConcreteRouteFilter::RouteStrings() const noexcept -> const std::set<std::string>&
    {
        return routes;
    }

    bool ConcreteRouteFilter::ApplicableToAircraft(const Euroscope::EuroscopeAircraft& aircraft) const noexcept
    {
        return std::find_if(
                       routes.begin(), routes.end(),
                       [&aircraft](const std::string& route) {
                           return aircraft.RouteString().find(route) != std::string::npos;
                       }
               ) != routes.end();
    }

    auto ConcreteRouteFilter::FilterDescription() const noexcept -> std::string
    {
        std::string description = "On route(s): ";
        for (const auto& route: routes) {
            description += route + ", ";
        }

        return description.substr(0, description.size() - 2);
    }
}// namespace ECFMP::FlowMeasure
