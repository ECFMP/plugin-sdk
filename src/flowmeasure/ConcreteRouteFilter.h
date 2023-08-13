#pragma once
#include "ECFMP/flowmeasure/RouteFilter.h"

namespace ECFMP::FlowMeasure {
    class ConcreteRouteFilter : public RouteFilter
    {
        public:
        explicit ConcreteRouteFilter(std::set<std::string> routes);
        [[nodiscard]] auto RouteStrings() const noexcept -> const std::set<std::string>& override;
        [[nodiscard]] auto ApplicableToAircraft(const Euroscope::EuroscopeAircraft& aircraft) const noexcept
                -> bool override;
        [[nodiscard]] auto FilterDescription() const noexcept -> std::string override;

        private:
        // The route strings
        std::set<std::string> routes;
    };
}// namespace ECFMP::FlowMeasure
