#include "ConcreteFlowMeasureFilters.h"
#include "flow-sdk/AirportFilter.h"
#include "flow-sdk/FlightInformationRegion.h"

namespace FlowSdk::FlowMeasure {

    ConcreteFlowMeasureFilters::ConcreteFlowMeasureFilters(
            std::list<std::shared_ptr<AirportFilter>> airportFilters,
            std::list<std::shared_ptr<EventFilter>> eventFilters, std::list<std::shared_ptr<RouteFilter>> routeFilters,
            std::list<std::shared_ptr<LevelFilter>> levelFilters
    )
        : airportFilters(std::move(airportFilters)), eventFilters(std::move(eventFilters)),
          routeFilters(std::move(routeFilters)), levelFilters(std::move(levelFilters))
    {}

    auto ConcreteFlowMeasureFilters::ApplicableToAirport(const std::string& airport) const noexcept -> bool
    {
        return FirstAirportFilter([&airport](const AirportFilter& filter) -> bool {
                   return filter.ApplicableToAirport(airport);
               })
                != nullptr;
    }

    void ConcreteFlowMeasureFilters::ForEachAirportFilter(const std::function<void(const AirportFilter&)>& callback
    ) const noexcept
    {
        for (const auto& filter: airportFilters) {
            callback(*filter);
        }
    }

    void ConcreteFlowMeasureFilters::ForEachEventFilter(const std::function<void(const EventFilter&)>& callback
    ) const noexcept
    {
        for (const auto& filter: eventFilters) {
            callback(*filter);
        }
    }

    void ConcreteFlowMeasureFilters::ForEachLevelFilter(const std::function<void(const LevelFilter&)>& callback
    ) const noexcept
    {
        for (const auto& filter: levelFilters) {
            callback(*filter);
        }
    }

    void ConcreteFlowMeasureFilters::ForEachRouteFilter(const std::function<void(const RouteFilter&)>& callback
    ) const noexcept
    {
        for (const auto& filter: routeFilters) {
            callback(*filter);
        }
    }

    auto ConcreteFlowMeasureFilters::FirstAirportFilter(const std::function<bool(const AirportFilter&)>& callback
    ) const noexcept -> std::shared_ptr<AirportFilter>
    {
        auto filter = std::find_if(airportFilters.begin(), airportFilters.end(), [&callback](const auto& item) {
            return callback(*item);
        });
        return filter == airportFilters.cend() ? nullptr : *filter;
    }

    auto ConcreteFlowMeasureFilters::FirstEventFilter(const std::function<bool(const EventFilter&)>& callback
    ) const noexcept -> std::shared_ptr<EventFilter>
    {
        auto filter = std::find_if(eventFilters.begin(), eventFilters.end(), [&callback](const auto& item) {
            return callback(*item);
        });
        return filter == eventFilters.cend() ? nullptr : *filter;
    }

    auto ConcreteFlowMeasureFilters::FirstLevelFilter(const std::function<bool(const LevelFilter&)>& callback
    ) const noexcept -> std::shared_ptr<LevelFilter>
    {
        auto filter = std::find_if(levelFilters.begin(), levelFilters.end(), [&callback](const auto& item) {
            return callback(*item);
        });
        return filter == levelFilters.cend() ? nullptr : *filter;
    }

    auto ConcreteFlowMeasureFilters::FirstRouteFilter(const std::function<bool(const RouteFilter&)>& callback
    ) const noexcept -> std::shared_ptr<RouteFilter>
    {
        auto filter = std::find_if(routeFilters.begin(), routeFilters.end(), [&callback](const auto& item) {
            return callback(*item);
        });
        return filter == routeFilters.cend() ? nullptr : *filter;
    }

    auto ConcreteFlowMeasureFilters::AirportFilters() const noexcept -> const std::list<std::shared_ptr<AirportFilter>>&
    {
        return airportFilters;
    }

    auto ConcreteFlowMeasureFilters::EventFilters() const noexcept -> const std::list<std::shared_ptr<EventFilter>>&
    {
        return eventFilters;
    }

    auto ConcreteFlowMeasureFilters::LevelFilters() const noexcept -> const std::list<std::shared_ptr<LevelFilter>>&
    {
        return levelFilters;
    }

    auto ConcreteFlowMeasureFilters::RouteFilters() const noexcept -> const std::list<std::shared_ptr<RouteFilter>>&
    {
        return routeFilters;
    }
}// namespace FlowSdk::FlowMeasure
