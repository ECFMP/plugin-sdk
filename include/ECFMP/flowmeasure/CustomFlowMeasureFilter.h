#pragma once

namespace EuroScopePlugIn {
    class CFlightPlan;
    class CRadarTarget;
}// namespace EuroScopePlugIn

namespace ECFMP::FlowMeasure {
    class FlowMeasure;

    /**
     * A class that consumers of the SDK can implement to provide a custom filter for flow measures.
     */
    class CustomFlowMeasureFilter
    {
        public:
        virtual ~CustomFlowMeasureFilter() = default;

        /**
         * Returns true if the given aircraft is applicable to the flow measure.
         * @return
         */
        [[nodiscard]] virtual auto ApplicableToAircraft(
                const EuroScopePlugIn::CFlightPlan& flightplan, const EuroScopePlugIn::CRadarTarget& radarTarget,
                const FlowMeasure& flowMeasure
        ) const noexcept -> bool = 0;
    };
}// namespace ECFMP::FlowMeasure
