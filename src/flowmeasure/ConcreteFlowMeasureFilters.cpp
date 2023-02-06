#include "ConcreteFlowMeasureFilters.h"
#include "flow-sdk/AirportFilter.h"
#include "flow-sdk/FlightInformationRegion.h"

namespace FlowSdk::FlowMeasure {

    ConcreteFlowMeasureFilters::ConcreteFlowMeasureFilters(
            std::list<std::shared_ptr<AirportFilter>> airportFilters,
            std::list<std::shared_ptr<EventFilter>> eventFilters, std::list<std::shared_ptr<RouteFilter>> routeFilters,
            std::list<std::shared_ptr<LevelFilter>> levelFilters,
            std::list<std::shared_ptr<FlightInformationRegion::FlightInformationRegion>> firs)
        : airportFilters(std::move(airportFilters)), eventFilters(std::move(eventFilters)),
          routeFilters(std::move(routeFilters)), levelFilters(std::move(levelFilters)), firs(std::move(firs))
    {}

    auto ConcreteFlowMeasureFilters::ApplicableToAirport(const std::string& airport) const noexcept -> bool
    {
        return FirstAirportFilter([&airport](const AirportFilter& filter) -> bool {
                   return filter.ApplicableToAirport(airport);
               })
                != nullptr;
    }

    auto ConcreteFlowMeasureFilters::ApplicableToFlightInformationRegion(int firId) const noexcept -> bool
    {
        auto fir = std::find_if(firs.begin(), firs.end(), [&firId](const auto& item) {
            return item->Id() == firId;
        });
        return fir != firs.cend();
    }

    auto ConcreteFlowMeasureFilters::ApplicableToFlightInformationRegion(
            const FlightInformationRegion::FlightInformationRegion& fir) const noexcept -> bool
    {
        return ApplicableToFlightInformationRegion(fir.Id());
    }

    auto
    ConcreteFlowMeasureFilters::ApplicableToFlightInformationRegion(const std::string& firIdentifier) const noexcept
            -> bool
    {
        auto fir = std::find_if(firs.begin(), firs.end(), [&firIdentifier](const auto& item) {
            return item->Identifier() == firIdentifier;
        });
        return fir != firs.cend();
    }

    void ConcreteFlowMeasureFilters::ForEachAirportFilter(
            const std::function<void(const AirportFilter&)>& callback) const noexcept
    {
        for (const auto& filter: airportFilters) {
            callback(*filter);
        }
    }

    void ConcreteFlowMeasureFilters::ForEachEventFilter(
            const std::function<void(const EventFilter&)>& callback) const noexcept
    {
        for (const auto& filter: eventFilters) {
            callback(*filter);
        }
    }

    void ConcreteFlowMeasureFilters::ForEachLevelFilter(
            const std::function<void(const LevelFilter&)>& callback) const noexcept
    {
        for (const auto& filter: levelFilters) {
            callback(*filter);
        }
    }

    void ConcreteFlowMeasureFilters::ForEachRouteFilter(
            const std::function<void(const RouteFilter&)>& callback) const noexcept
    {
        for (const auto& filter: routeFilters) {
            callback(*filter);
        }
    }

    auto ConcreteFlowMeasureFilters::FirstAirportFilter(
            const std::function<bool(const AirportFilter&)>& callback) const noexcept -> std::shared_ptr<AirportFilter>
    {
        auto filter = std::find_if(airportFilters.begin(), airportFilters.end(), [&callback](const auto& item) {
            return callback(*item);
        });
        return filter == airportFilters.cend() ? nullptr : *filter;
    }

    auto
    ConcreteFlowMeasureFilters::FirstEventFilter(const std::function<bool(const EventFilter&)>& callback) const noexcept
            -> std::shared_ptr<EventFilter>
    {
        auto filter = std::find_if(eventFilters.begin(), eventFilters.end(), [&callback](const auto& item) {
            return callback(*item);
        });
        return filter == eventFilters.cend() ? nullptr : *filter;
    }

    auto
    ConcreteFlowMeasureFilters::FirstLevelFilter(const std::function<bool(const LevelFilter&)>& callback) const noexcept
            -> std::shared_ptr<LevelFilter>
    {
        auto filter = std::find_if(levelFilters.begin(), levelFilters.end(), [&callback](const auto& item) {
            return callback(*item);
        });
        return filter == levelFilters.cend() ? nullptr : *filter;
    }

    auto
    ConcreteFlowMeasureFilters::FirstRouteFilter(const std::function<bool(const RouteFilter&)>& callback) const noexcept
            -> std::shared_ptr<RouteFilter>
    {
        auto filter = std::find_if(routeFilters.begin(), routeFilters.end(), [&callback](const auto& item) {
            return callback(*item);
        });
        return filter == routeFilters.cend() ? nullptr : *filter;
    }
}// namespace FlowSdk::FlowMeasure
