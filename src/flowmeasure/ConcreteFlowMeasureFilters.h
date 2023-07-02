#pragma once
#include "flow-sdk/FlowMeasureFilters.h"

namespace FlowSdk::FlightInformationRegion {
    class FlightInformationRegion;
}// namespace FlowSdk::FlightInformationRegion

namespace FlowSdk::FlowMeasure {
    /**
     * An implementation of the flow measure filters.
     */
    class ConcreteFlowMeasureFilters : public FlowMeasureFilters
    {
        public:
        ConcreteFlowMeasureFilters(
                std::list<std::shared_ptr<AirportFilter>> airportFilters,
                std::list<std::shared_ptr<EventFilter>> eventFilters,
                std::list<std::shared_ptr<RouteFilter>> routeFilters,
                std::list<std::shared_ptr<LevelFilter>> levelFilters
        );
        [[nodiscard]] auto AirportFilters() const noexcept -> const std::list<std::shared_ptr<AirportFilter>>&;
        [[nodiscard]] auto EventFilters() const noexcept -> const std::list<std::shared_ptr<EventFilter>>&;
        [[nodiscard]] auto RouteFilters() const noexcept -> const std::list<std::shared_ptr<RouteFilter>>&;
        [[nodiscard]] auto LevelFilters() const noexcept -> const std::list<std::shared_ptr<LevelFilter>>&;
        [[nodiscard]] auto ApplicableToAirport(const std::string& airfield) const noexcept -> bool override;
        void ForEachAirportFilter(const std::function<void(const AirportFilter&)>& callback) const noexcept override;
        void ForEachEventFilter(const std::function<void(const EventFilter&)>& callback) const noexcept override;
        void ForEachLevelFilter(const std::function<void(const LevelFilter&)>& callback) const noexcept override;
        void ForEachRouteFilter(const std::function<void(const RouteFilter&)>& callback) const noexcept override;
        [[nodiscard]] auto FirstAirportFilter(const std::function<bool(const AirportFilter&)>& callback) const noexcept
                -> std::shared_ptr<AirportFilter> override;
        [[nodiscard]] auto FirstEventFilter(const std::function<bool(const EventFilter&)>& callback) const noexcept
                -> std::shared_ptr<EventFilter> override;
        [[nodiscard]] auto FirstLevelFilter(const std::function<bool(const LevelFilter&)>& callback) const noexcept
                -> std::shared_ptr<LevelFilter> override;
        [[nodiscard]] auto FirstRouteFilter(const std::function<bool(const RouteFilter&)>& callback) const noexcept
                -> std::shared_ptr<RouteFilter> override;

        private:
        // All the airport filters
        std::list<std::shared_ptr<AirportFilter>> airportFilters;

        // All the event filters
        std::list<std::shared_ptr<EventFilter>> eventFilters;

        // All the route filters
        std::list<std::shared_ptr<RouteFilter>> routeFilters;

        // All the level filters
        std::list<std::shared_ptr<LevelFilter>> levelFilters;
    };
}// namespace FlowSdk::FlowMeasure
