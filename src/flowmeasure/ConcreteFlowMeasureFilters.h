#pragma once
#include "ECFMP/flowmeasure/FlowMeasureFilters.h"

namespace ECFMP {
    namespace Euroscope {
        class EuroscopeAircraftFactory;
    }// namespace Euroscope
    namespace FlightInformationRegion {
        class FlightInformationRegion;
    }// namespace FlightInformationRegion
}// namespace ECFMP

namespace ECFMP::FlowMeasure {
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
                std::list<std::shared_ptr<LevelRangeFilter>> levelFilters,
                std::list<std::shared_ptr<MultipleLevelFilter>> multipleLevelFilters,
                std::list<std::shared_ptr<RangeToDestinationFilter>> rangeToDestinationFilters,
                std::shared_ptr<const Euroscope::EuroscopeAircraftFactory> aircraftFactory
        );
        [[nodiscard]] auto AirportFilters() const noexcept -> const std::list<std::shared_ptr<AirportFilter>>&;
        [[nodiscard]] auto EventFilters() const noexcept -> const std::list<std::shared_ptr<EventFilter>>&;
        [[nodiscard]] auto RouteFilters() const noexcept -> const std::list<std::shared_ptr<RouteFilter>>&;
        [[nodiscard]] auto LevelFilters() const noexcept -> const std::list<std::shared_ptr<LevelRangeFilter>>&;
        [[nodiscard]] auto MultipleLevelFilters() const noexcept
                -> const std::list<std::shared_ptr<MultipleLevelFilter>>&;
        [[nodiscard]] auto RangeToDestinationFilters() const noexcept
                -> const std::list<std::shared_ptr<RangeToDestinationFilter>>&;
        [[nodiscard]] auto ApplicableToAirport(const std::string& airfield) const noexcept -> bool override;
        auto ApplicableToAircraft(
                const EuroScopePlugIn::CFlightPlan& flightplan, const EuroScopePlugIn::CRadarTarget& radarTarget
        ) const -> bool override;
        void ForEachAirportFilter(const std::function<void(const AirportFilter&)>& callback) const noexcept override;
        void ForEachEventFilter(const std::function<void(const EventFilter&)>& callback) const noexcept override;
        void ForEachLevelFilter(const std::function<void(const LevelRangeFilter&)>& callback) const noexcept override;
        void ForEachMultipleLevelFilter(const std::function<void(const MultipleLevelFilter&)>& callback
        ) const noexcept override;
        void ForEachRouteFilter(const std::function<void(const RouteFilter&)>& callback) const noexcept override;
        void ForEachRangeToDestinationFilter(const std::function<void(const RangeToDestinationFilter&)>& callback
        ) const noexcept override;
        [[nodiscard]] auto FirstAirportFilter(const std::function<bool(const AirportFilter&)>& callback) const noexcept
                -> std::shared_ptr<AirportFilter> override;
        [[nodiscard]] auto FirstEventFilter(const std::function<bool(const EventFilter&)>& callback) const noexcept
                -> std::shared_ptr<EventFilter> override;
        [[nodiscard]] auto FirstLevelFilter(const std::function<bool(const LevelRangeFilter&)>& callback) const noexcept
                -> std::shared_ptr<LevelRangeFilter> override;
        [[nodiscard]] auto FirstMultipleLevelFilter(const std::function<bool(const MultipleLevelFilter&)>& callback
        ) const noexcept -> std::shared_ptr<MultipleLevelFilter> override;
        [[nodiscard]] auto FirstRouteFilter(const std::function<bool(const RouteFilter&)>& callback) const noexcept
                -> std::shared_ptr<RouteFilter> override;
        [[nodiscard]] auto
        FirstRangeToDestinationFilter(const std::function<bool(const RangeToDestinationFilter&)>& callback
        ) const noexcept -> std::shared_ptr<RangeToDestinationFilter> override;
        [[nodiscard]] auto FilterDescriptions() const noexcept -> std::vector<std::string> override;

        private:
        // All the airport filters
        std::list<std::shared_ptr<AirportFilter>> airportFilters;

        // All the event filters
        std::list<std::shared_ptr<EventFilter>> eventFilters;

        // All the route filters
        std::list<std::shared_ptr<RouteFilter>> routeFilters;

        // All the level filters
        std::list<std::shared_ptr<LevelRangeFilter>> levelFilters;

        // All the multiple level filters
        std::list<std::shared_ptr<MultipleLevelFilter>> multipleLevelFilters;

        // All the range to destination filters
        std::list<std::shared_ptr<RangeToDestinationFilter>> rangeToDestinationFilters;

        // For wrapping euroscope classes for testing
        std::shared_ptr<const Euroscope::EuroscopeAircraftFactory> aircraftFactory;
    };
}// namespace ECFMP::FlowMeasure
