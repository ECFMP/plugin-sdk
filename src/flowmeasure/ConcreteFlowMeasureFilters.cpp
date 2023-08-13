#include "ConcreteFlowMeasureFilters.h"
#include "ECFMP/flightinformationregion/FlightInformationRegion.h"
#include "ECFMP/flowmeasure/AirportFilter.h"
#include "ECFMP/flowmeasure/EventFilter.h"
#include "ECFMP/flowmeasure/LevelRangeFilter.h"
#include "ECFMP/flowmeasure/MultipleLevelFilter.h"
#include "ECFMP/flowmeasure/RangeToDestinationFilter.h"
#include "ECFMP/flowmeasure/RouteFilter.h"
#include "euroscope/EuroscopeAircraftFactory.h"
#include <algorithm>

namespace ECFMP::FlowMeasure {

    ConcreteFlowMeasureFilters::ConcreteFlowMeasureFilters(
            std::list<std::shared_ptr<AirportFilter>> airportFilters,
            std::list<std::shared_ptr<EventFilter>> eventFilters, std::list<std::shared_ptr<RouteFilter>> routeFilters,
            std::list<std::shared_ptr<LevelRangeFilter>> levelFilters,
            std::list<std::shared_ptr<MultipleLevelFilter>> multipleLevelFilters,
            std::list<std::shared_ptr<RangeToDestinationFilter>> rangeToDestinationFilters,
            std::shared_ptr<const Euroscope::EuroscopeAircraftFactory> aircraftFactory
    )
        : airportFilters(std::move(airportFilters)), eventFilters(std::move(eventFilters)),
          routeFilters(std::move(routeFilters)), levelFilters(std::move(levelFilters)),
          multipleLevelFilters(std::move(multipleLevelFilters)),
          rangeToDestinationFilters(std::move(rangeToDestinationFilters)), aircraftFactory(std::move(aircraftFactory))
    {
        assert(this->aircraftFactory != nullptr && "The aircraft factory cannot be null.");
    }

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

    void ConcreteFlowMeasureFilters::ForEachLevelFilter(const std::function<void(const LevelRangeFilter&)>& callback
    ) const noexcept
    {
        for (const auto& filter: levelFilters) {
            callback(*filter);
        }
    }

    void ConcreteFlowMeasureFilters::ForEachMultipleLevelFilter(
            const std::function<void(const MultipleLevelFilter&)>& callback
    ) const noexcept
    {
        for (const auto& filter: multipleLevelFilters) {
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

    void ConcreteFlowMeasureFilters::ForEachRangeToDestinationFilter(
            const std::function<void(const RangeToDestinationFilter&)>& callback
    ) const noexcept
    {
        for (const auto& filter: rangeToDestinationFilters) {
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

    auto ConcreteFlowMeasureFilters::FirstLevelFilter(const std::function<bool(const LevelRangeFilter&)>& callback
    ) const noexcept -> std::shared_ptr<LevelRangeFilter>
    {
        auto filter = std::find_if(levelFilters.begin(), levelFilters.end(), [&callback](const auto& item) {
            return callback(*item);
        });
        return filter == levelFilters.cend() ? nullptr : *filter;
    }

    auto
    ConcreteFlowMeasureFilters::FirstMultipleLevelFilter(const std::function<bool(const MultipleLevelFilter&)>& callback
    ) const noexcept -> std::shared_ptr<MultipleLevelFilter>
    {
        auto filter =
                std::find_if(multipleLevelFilters.begin(), multipleLevelFilters.end(), [&callback](const auto& item) {
                    return callback(*item);
                });
        return filter == multipleLevelFilters.cend() ? nullptr : *filter;
    }

    auto ConcreteFlowMeasureFilters::FirstRouteFilter(const std::function<bool(const RouteFilter&)>& callback
    ) const noexcept -> std::shared_ptr<RouteFilter>
    {
        auto filter = std::find_if(routeFilters.begin(), routeFilters.end(), [&callback](const auto& item) {
            return callback(*item);
        });
        return filter == routeFilters.cend() ? nullptr : *filter;
    }

    auto ConcreteFlowMeasureFilters::FirstRangeToDestinationFilter(
            const std::function<bool(const RangeToDestinationFilter&)>& callback
    ) const noexcept -> std::shared_ptr<RangeToDestinationFilter>
    {
        auto filter = std::find_if(
                rangeToDestinationFilters.begin(), rangeToDestinationFilters.end(),
                [&callback](const auto& item) {
                    return callback(*item);
                }
        );
        return filter == rangeToDestinationFilters.cend() ? nullptr : *filter;
    }

    auto ConcreteFlowMeasureFilters::AirportFilters() const noexcept -> const std::list<std::shared_ptr<AirportFilter>>&
    {
        return airportFilters;
    }

    auto ConcreteFlowMeasureFilters::EventFilters() const noexcept -> const std::list<std::shared_ptr<EventFilter>>&
    {
        return eventFilters;
    }

    auto ConcreteFlowMeasureFilters::LevelFilters() const noexcept
            -> const std::list<std::shared_ptr<LevelRangeFilter>>&
    {
        return levelFilters;
    }

    auto ConcreteFlowMeasureFilters::MultipleLevelFilters() const noexcept
            -> const std::list<std::shared_ptr<MultipleLevelFilter>>&
    {
        return multipleLevelFilters;
    }

    auto ConcreteFlowMeasureFilters::RouteFilters() const noexcept -> const std::list<std::shared_ptr<RouteFilter>>&
    {
        return routeFilters;
    }

    auto ConcreteFlowMeasureFilters::RangeToDestinationFilters() const noexcept
            -> const std::list<std::shared_ptr<RangeToDestinationFilter>>&
    {
        return rangeToDestinationFilters;
    }

    /**
     * The logic between different filters is logical AND, with values within filters being logical OR.
     * @return
     */
    bool ConcreteFlowMeasureFilters::ApplicableToAircraft(
            const EuroScopePlugIn::CFlightPlan& flightplan, const EuroScopePlugIn::CRadarTarget& radarTarget
    ) const
    {
        const auto euroscopeAircraft = aircraftFactory->Make(flightplan, radarTarget);

        // Check if the aircraft is not applicable to any of the airport filters
        const auto passesAirportFilters =
                std::all_of(airportFilters.cbegin(), airportFilters.cend(), [&euroscopeAircraft](const auto& filter) {
                    return filter->ApplicableToAircraft(*euroscopeAircraft);
                });

        if (!passesAirportFilters) {
            return false;
        }

        // Check if the aircraft is not applicable to any of the event filters
        const auto passesEventFilters =
                std::all_of(eventFilters.cbegin(), eventFilters.cend(), [&euroscopeAircraft](const auto& filter) {
                    return filter->ApplicableToAircraft(*euroscopeAircraft);
                });

        if (!passesEventFilters) {
            return false;
        }

        // Check if the aircraft is not applicable to any of the level filters

        const auto passesLevelFilters =
                std::all_of(levelFilters.cbegin(), levelFilters.cend(), [&euroscopeAircraft](const auto& filter) {
                    return filter->ApplicableToAircraft(*euroscopeAircraft);
                });

        if (!passesLevelFilters) {
            return false;
        }

        // Check if the aircraft is not applicable to any of the multiple level filters
        const auto passesMultipleLevelFilters = std::all_of(
                multipleLevelFilters.cbegin(), multipleLevelFilters.cend(),
                [&euroscopeAircraft](const auto& filter) {
                    return filter->ApplicableToAircraft(*euroscopeAircraft);
                }
        );

        if (!passesMultipleLevelFilters) {
            return false;
        }

        // Check if the aircraft is not applicable to any of the route filters
        const auto passesRouteFilters =
                std::all_of(routeFilters.cbegin(), routeFilters.cend(), [&euroscopeAircraft](const auto& filter) {
                    return filter->ApplicableToAircraft(*euroscopeAircraft);
                });

        if (!passesRouteFilters) {
            return false;
        }

        // Check if the aircraft is not applicable to any of the range to destination filters
        const auto passesRangeToDestinationFilters = std::all_of(
                rangeToDestinationFilters.cbegin(), rangeToDestinationFilters.cend(),
                [&euroscopeAircraft](const auto& filter) {
                    return filter->ApplicableToAircraft(*euroscopeAircraft);
                }
        );

        return passesRangeToDestinationFilters;
    }

    auto ConcreteFlowMeasureFilters::FilterDescriptions() const noexcept -> std::vector<std::string>
    {
        std::vector<std::string> descriptions;
        descriptions.reserve(
                airportFilters.size() + eventFilters.size() + levelFilters.size() + multipleLevelFilters.size()
                + routeFilters.size() + rangeToDestinationFilters.size()
        );

        std::transform(
                airportFilters.cbegin(), airportFilters.cend(), std::back_inserter(descriptions),
                [](const auto& filter) {
                    return filter->FilterDescription();
                }
        );
        std::transform(
                eventFilters.cbegin(), eventFilters.cend(), std::back_inserter(descriptions),
                [](const auto& filter) {
                    return filter->FilterDescription();
                }
        );
        std::transform(
                levelFilters.cbegin(), levelFilters.cend(), std::back_inserter(descriptions),
                [](const auto& filter) {
                    return filter->FilterDescription();
                }
        );
        std::transform(
                multipleLevelFilters.cbegin(), multipleLevelFilters.cend(), std::back_inserter(descriptions),
                [](const auto& filter) {
                    return filter->FilterDescription();
                }
        );
        std::transform(
                routeFilters.cbegin(), routeFilters.cend(), std::back_inserter(descriptions),
                [](const auto& filter) {
                    return filter->FilterDescription();
                }
        );
        std::transform(
                rangeToDestinationFilters.cbegin(), rangeToDestinationFilters.cend(), std::back_inserter(descriptions),
                [](const auto& filter) {
                    return filter->FilterDescription();
                }
        );

        return descriptions;
    }
}// namespace ECFMP::FlowMeasure
