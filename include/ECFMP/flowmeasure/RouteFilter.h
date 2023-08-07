#pragma once
#include "ChecksAircraftApplicability.h"

namespace ECFMP::FlowMeasure {
    /**
     * A filter that pertains to aircraft on particular routes.
     */
    class RouteFilter : public ChecksAircraftApplicability
    {
        public:
        virtual ~RouteFilter() = default;
        [[nodiscard]] virtual auto RouteStrings() const noexcept -> const std::set<std::string>& = 0;
    };
}// namespace ECFMP::FlowMeasure
