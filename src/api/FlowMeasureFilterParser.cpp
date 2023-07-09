#include "FlowMeasureFilterParser.h"
#include "ECFMP/flightinformationregion/FlightInformationRegion.h"
#include "ECFMP/flowmeasure/EventFilter.h"
#include "ECFMP/flowmeasure/FlowMeasureFilters.h"
#include "ECFMP/flowmeasure/LevelRangeFilter.h"
#include "ECFMP/flowmeasure/MultipleLevelFilter.h"
#include "ECFMP/flowmeasure/RangeToDestinationFilter.h"
#include "ECFMP/log/Logger.h"
#include "flowmeasure/ConcreteAirportFilter.h"
#include "flowmeasure/ConcreteEventFilter.h"
#include "flowmeasure/ConcreteFlowMeasureFilters.h"
#include "flowmeasure/ConcreteLevelRangeFilter.h"
#include "flowmeasure/ConcreteMultipleLevelFilter.h"
#include "flowmeasure/ConcreteRangeToDestinationFilter.h"
#include "flowmeasure/ConcreteRouteFilter.h"
#include "nlohmann/json.hpp"

namespace ECFMP::Api {
    FlowMeasureFilterParser::FlowMeasureFilterParser(const std::shared_ptr<ECFMP::Log::Logger>& logger) : logger(logger)
    {
        assert(this->logger && "Logger cannot be null");
    }

    auto FlowMeasureFilterParser::Parse(const nlohmann::json& data, const InternalEventCollection& events) const
            -> std::unique_ptr<FlowMeasure::FlowMeasureFilters>
    {
        // The JSON must be an array, or else return nullptr
        if (!data.is_array()) {
            logger->Warning("FlowMeasureFilterParser::Parse: Json is not an array");
            return nullptr;
        }

        std::list<std::shared_ptr<FlowMeasure::AirportFilter>> airportFilters;
        std::list<std::shared_ptr<FlowMeasure::LevelRangeFilter>> levelRangeFilters;
        std::list<std::shared_ptr<FlowMeasure::MultipleLevelFilter>> multipleLevelFilters;
        std::list<std::shared_ptr<FlowMeasure::EventFilter>> eventFilters;
        std::list<std::shared_ptr<FlowMeasure::RouteFilter>> routeFilters;
        std::list<std::shared_ptr<FlowMeasure::RangeToDestinationFilter>> rangeFilters;

        for (const auto& filter: data) {
            // Json must contain a value field otherwise return nullptr
            if (!filter.contains("value")) {
                logger->Warning("FlowMeasureFilterParser::Parse: Json does not contain a value field");
                return nullptr;
            }

            // Json must contain a type field otherwise return nullptr
            if (!filter.contains("type") || !filter["type"].is_string()) {
                logger->Warning(
                        "FlowMeasureFilterParser::Parse: Json does not contain a type field or the type field is "
                        "not a string"
                );
                return nullptr;
            }

            // If the filter type is ADEP OR ADES, then the filter type is an AirportFilter
            if (filter["type"] == "ADEP" || filter["type"] == "ADES") {
                auto airportFilter = CreateAirportFilter(filter["value"], filter["type"].get<std::string>());
                if (!airportFilter) {
                    logger->Warning("FlowMeasureFilterParser::Parse: Could not create airport filter");
                    return nullptr;
                }

                airportFilters.push_back(std::move(airportFilter));
                continue;
            }

            // If the filter is type level_above, level_below, then the filter type is a LevelFilter
            else if (filter["type"] == "level_above" || filter["type"] == "level_below") {
                auto filterType = filter["type"] == "level_above" ? FlowMeasure::LevelRangeFilterType::AtOrAbove
                                                                  : FlowMeasure::LevelRangeFilterType::AtOrBelow;
                auto levelFilter = CreateLevelRangeFilter(filter["value"], filterType);
                if (!levelFilter) {
                    logger->Warning("FlowMeasureFilterParser::Parse: Could not create level filter");
                    return nullptr;
                }

                levelRangeFilters.push_back(std::move(levelFilter));
                continue;
            }

            // If the filter is type level, then the filter is an array, each item of which makes a LevelFilter
            else if (filter["type"] == "level") {
                auto levelFilter = CreateMultipleLevelFilter(filter["value"]);
                if (!levelFilter) {
                    logger->Warning("FlowMeasureFilterParser::Parse: Could not create multiple level filter");
                    return nullptr;
                }

                multipleLevelFilters.push_back(std::move(levelFilter));
                continue;
            }

            // If the filter is type member_event or member_not_event, then the filter type is an EventFilter
            else if (filter["type"] == "member_event" || filter["type"] == "member_not_event") {
                auto eventFilter = CreateEventFilter(filter, events);
                if (!eventFilter) {
                    logger->Warning("FlowMeasureFilterParser::Parse: Could not create event filter");
                    return nullptr;
                }

                eventFilters.push_back(std::move(eventFilter));
                continue;
            }

            // If the filter is type waypoint, then the filter type is a RouteFilter
            else if (filter["type"] == "waypoint") {
                auto routeFilter = CreateRouteFilter(filter.at("value"));
                if (!routeFilter) {
                    logger->Warning("FlowMeasureFilterParser::Parse: Could not create route filter");
                    return nullptr;
                }

                routeFilters.push_back(std::move(routeFilter));
                continue;
            }

            else if (filter["type"] == "range_to_destination") {
                auto rangeFilter = CreateRangeToDestinationFilter(filter.at("value"));
                if (!rangeFilter) {
                    logger->Warning("FlowMeasureFilterParser::Parse: Could not create range to destination filter");
                    return nullptr;
                }

                rangeFilters.push_back(std::move(rangeFilter));
            }

            logger->Warning("FlowMeasureFilterParser::Parse: Unknown filter type");
        }

        return std::make_unique<FlowMeasure::ConcreteFlowMeasureFilters>(
                std::move(airportFilters), std::move(eventFilters), std::move(routeFilters),
                std::move(levelRangeFilters), std::move(multipleLevelFilters), std::move(rangeFilters)
        );
    }

    auto FlowMeasureFilterParser::CreateAirportFilter(const nlohmann::json& data, const std::string& type) const
            -> std::shared_ptr<FlowMeasure::AirportFilter>
    {
        // Convert type to enum
        auto filterType = type == "ADES" ? FlowMeasure::AirportFilterType::Destination
                                         : FlowMeasure::AirportFilterType::Departure;

        // JSON should be an array, each value must be a string
        if (!data.is_array()) {
            logger->Warning("FlowMeasureFilterParser::CreateAirportFilter: Json is not an array");
            return nullptr;
        }

        std::set<std::string> airports;
        for (const auto& airport: data) {
            if (!airport.is_string()) {
                logger->Warning("FlowMeasureFilterParser::CreateAirportFilter: Json value is not a string");
                return nullptr;
            }

            airports.insert(airport);
        }

        return std::make_shared<FlowMeasure::ConcreteAirportFilter>(airports, filterType);
    }

    auto FlowMeasureFilterParser::CreateLevelRangeFilter(
            const nlohmann::json& data, FlowMeasure::LevelRangeFilterType type
    ) const -> std::shared_ptr<FlowMeasure::LevelRangeFilter>
    {
        if (!data.is_number_integer()) {
            logger->Warning("FlowMeasureFilterParser::CreateLevelRangeFilter: Json value is not an integer");
            return nullptr;
        }

        return std::make_shared<FlowMeasure::ConcreteLevelRangeFilter>(type, data.get<int>());
    }

    auto FlowMeasureFilterParser::CreateMultipleLevelFilter(const nlohmann::json& data) const
            -> std::shared_ptr<FlowMeasure::MultipleLevelFilter>
    {
        if (!data.is_array()) {
            logger->Warning("FlowMeasureFilterParser::CreateMultipleLevelFilter: Json is not an array");
            return nullptr;
        }

        std::vector<int> levels;
        for (const auto& level: data) {
            if (!level.is_number_integer()) {
                logger->Warning("FlowMeasureFilterParser::CreateMultipleLevelFilter: Json value is not an integer");
                return nullptr;
            }

            levels.push_back(level.get<int>());
        }

        return std::make_shared<FlowMeasure::ConcreteMultipleLevelFilter>(levels);
    }

    auto
    FlowMeasureFilterParser::CreateEventFilter(const nlohmann::json& data, const InternalEventCollection& events) const
            -> std::shared_ptr<FlowMeasure::EventFilter>
    {
        auto participation = data["type"] == "member_event" ? FlowMeasure::EventParticipation::Participating
                                                            : FlowMeasure::EventParticipation::NotParticipating;

        // JSON should contain a value field, which must be an integer
        if (!data["value"].is_number_integer()) {
            logger->Warning("FlowMeasureFilterParser::CreateEventFilter: Json does not contain a value field or the "
                            "value field is not an integer");
            return nullptr;
        }

        // Check the event exists in the collection
        const auto event = events.Get(data["value"].get<int>());
        if (!event) {
            logger->Warning("FlowMeasureFilterParser::CreateEventFilter: Event does not exist");
            return nullptr;
        }

        return std::make_shared<FlowMeasure::ConcreteEventFilter>(event, participation);
    }

    auto FlowMeasureFilterParser::CreateRouteFilter(const nlohmann::json& data) const
            -> std::shared_ptr<FlowMeasure::RouteFilter>
    {
        // JSON should be an array, each value must be a string
        if (!data.is_array()) {
            logger->Warning("FlowMeasureFilterParser::CreateRouteFilter: Json is not an array");
            return nullptr;
        }

        std::set<std::string> routes;
        for (const auto& route: data) {
            if (!route.is_string()) {
                logger->Warning("FlowMeasureFilterParser::CreateRouteFilter: Json value is not a string");
                return nullptr;
            }

            routes.insert(route);
        }

        return std::make_shared<FlowMeasure::ConcreteRouteFilter>(routes);
    }

    auto FlowMeasureFilterParser::CreateRangeToDestinationFilter(const nlohmann::json& data) const
            -> std::shared_ptr<FlowMeasure::RangeToDestinationFilter>
    {
        if (!data.is_number_integer()) {
            logger->Warning("FlowMeasureFilterParser::CreateRangeToDestinationFilter: Json value is not an integer");
            return nullptr;
        }

        return std::make_shared<FlowMeasure::ConcreteRangeToDestinationFilter>(data.get<int>());
    }
}// namespace ECFMP::Api
