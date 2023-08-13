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

        /**
         * Returns the set of route strings that this filter pertains to.
         * @return
         */
        [[nodiscard]] virtual auto RouteStrings() const noexcept -> const std::set<std::string>& = 0;

        /**
         * Description of the filter.
         */
        [[nodiscard]] virtual auto FilterDescription() const noexcept -> std::string = 0;
    };
}// namespace ECFMP::FlowMeasure
