#pragma once

namespace ECFMP::FlightInformationRegion {
    class FlightInformationRegion;
}// namespace ECFMP::FlightInformationRegion

namespace EuroScopePlugIn {
    class CFlightPlan;
    class CRadarTarget;
}// namespace EuroScopePlugIn

namespace ECFMP::FlowMeasure {

    class AirportFilter;
    class EventFilter;
    class LevelRangeFilter;
    class MultipleLevelFilter;
    class RangeToDestinationFilter;
    class RouteFilter;

    /**
     * Represents a collection of flow measure filters, along with some helper
     * methods for commonly used queries.
     *
     * It does not include any custom filters provided by the user.
     */
    class FlowMeasureFilters
    {
        public:
        virtual ~FlowMeasureFilters() = default;
        /**
         * Returns true if any of the airport filters make the flow measure applicable to
         * the given airport.
         */
        [[nodiscard]] virtual auto ApplicableToAirport(const std::string& airfield) const noexcept -> bool = 0;

        /**
         * Returns true if the flow measure is applicable to the given aircraft. It will be applicable if all
         * of the filters are applicable.
         */
        [[nodiscard]] virtual auto ApplicableToAircraft(
                const EuroScopePlugIn::CFlightPlan& flightplan, const EuroScopePlugIn::CRadarTarget& radarTarget
        ) const -> bool = 0;

        /**
         * Methods that allow for iteration of the filters.
         */
        virtual void ForEachAirportFilter(const std::function<void(const AirportFilter&)>& callback) const noexcept = 0;
        virtual void ForEachEventFilter(const std::function<void(const EventFilter&)>& callback) const noexcept = 0;
        virtual void ForEachLevelFilter(const std::function<void(const LevelRangeFilter&)>& callback
        ) const noexcept = 0;
        virtual void ForEachMultipleLevelFilter(const std::function<void(const MultipleLevelFilter&)>& callback
        ) const noexcept = 0;
        virtual void ForEachRouteFilter(const std::function<void(const RouteFilter&)>& callback) const noexcept = 0;
        virtual void
        ForEachRangeToDestinationFilter(const std::function<void(const RangeToDestinationFilter&)>& callback
        ) const noexcept = 0;

        /**
         * Returns the first filter where a condition is met.
         */
        [[nodiscard]] virtual auto FirstAirportFilter(const std::function<bool(const AirportFilter&)>& callback
        ) const noexcept -> std::shared_ptr<AirportFilter> = 0;
        [[nodiscard]] virtual auto FirstEventFilter(const std::function<bool(const EventFilter&)>& callback
        ) const noexcept -> std::shared_ptr<EventFilter> = 0;
        [[nodiscard]] virtual auto FirstLevelFilter(const std::function<bool(const LevelRangeFilter&)>& callback
        ) const noexcept -> std::shared_ptr<LevelRangeFilter> = 0;
        [[nodiscard]] virtual auto
        FirstMultipleLevelFilter(const std::function<bool(const MultipleLevelFilter&)>& callback) const noexcept
                -> std::shared_ptr<MultipleLevelFilter> = 0;
        [[nodiscard]] virtual auto FirstRouteFilter(const std::function<bool(const RouteFilter&)>& callback
        ) const noexcept -> std::shared_ptr<RouteFilter> = 0;
        [[nodiscard]] virtual auto
        FirstRangeToDestinationFilter(const std::function<bool(const RangeToDestinationFilter&)>& callback
        ) const noexcept -> std::shared_ptr<RangeToDestinationFilter> = 0;

        /**
         * Convenience method to return string description of the filters.
         */
        [[nodiscard]] virtual auto FilterDescriptions() const noexcept -> std::vector<std::string> = 0;
    };
}// namespace ECFMP::FlowMeasure
