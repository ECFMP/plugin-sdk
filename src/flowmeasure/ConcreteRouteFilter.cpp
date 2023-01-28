#include "ConcreteRouteFilter.h"

namespace FlowSdk::FlowMeasure {
    ConcreteRouteFilter::ConcreteRouteFilter(std::set<std::string> routes) : routes(std::move(routes))
    {}

    auto ConcreteRouteFilter::RouteStrings() const noexcept -> const std::set<std::string>&
    {
        return routes;
    }
}// namespace FlowSdk::FlowMeasure
