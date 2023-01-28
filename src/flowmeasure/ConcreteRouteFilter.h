#pragma once
#include "RouteFilter.h"

namespace FlowSdk::FlowMeasure {
    class ConcreteRouteFilter : public RouteFilter
    {
        public:
        explicit ConcreteRouteFilter(std::set<std::string> routes);
        [[nodiscard]] auto RouteStrings() const noexcept -> const std::set<std::string>& override;

        private:
        // The route strings
        std::set<std::string> routes;
    };
}// namespace FlowSdk::FlowMeasure
