#pragma once

namespace FlowSdk::FlowMeasure {
    /**
     * A filter that pertains to aircraft on particular routes.
     */
    class RouteFilter
    {
        public:
        virtual ~RouteFilter() = default;
        [[nodiscard]] virtual auto RouteStrings() const noexcept -> const std::set<std::string>& = 0;
    };
}// namespace FlowSdk::FlowMeasure
