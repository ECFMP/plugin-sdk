#include "FlowMeasureFilterParser.h"
#include "flow-sdk/EventFilter.h"
#include "flow-sdk/FlightInformationRegion.h"
#include "flow-sdk/FlowMeasureFilters.h"
#include "flow-sdk/LevelFilter.h"
#include "flow-sdk/Logger.h"
#include "flowmeasure/ConcreteAirportFilter.h"
#include "flowmeasure/ConcreteEventFilter.h"
#include "flowmeasure/ConcreteFlowMeasureFilters.h"
#include "flowmeasure/ConcreteLevelFilter.h"
#include "flowmeasure/ConcreteRouteFilter.h"
#include "nlohmann/json.hpp"

namespace FlowSdk::Api {
    FlowMeasureFilterParser::FlowMeasureFilterParser(const std::shared_ptr<FlowSdk::Log::Logger>& logger)
        : logger(logger)
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
        std::list<std::shared_ptr<FlowMeasure::LevelFilter>> levelFilters;
        std::list<std::shared_ptr<FlowMeasure::EventFilter>> eventFilters;
        std::list<std::shared_ptr<FlowMeasure::RouteFilter>> routeFilters;

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
                auto airportFilter = CreateAirportFilter(filter["value"]);
                if (!airportFilter) {
                    logger->Warning("FlowMeasureFilterParser::Parse: Could not create airport filter");
                    return nullptr;
                }

                airportFilters.push_back(std::move(airportFilter));
                continue;
            }

            // If the filter is type level_above, level_below, then the filter type is a LevelFilter
            else if (filter["type"] == "level_above" || filter["type"] == "level_below") {
                auto filterType = filter["type"] == "level_above" ? FlowMeasure::LevelFilterType::AtOrAbove
                                                                  : FlowMeasure::LevelFilterType::AtOrBelow;
                auto levelFilter = CreateLevelFilter(filter["value"], filterType);
                if (!levelFilter) {
                    logger->Warning("FlowMeasureFilterParser::Parse: Could not create level filter");
                    return nullptr;
                }

                levelFilters.push_back(std::move(levelFilter));
                continue;
            }

            // If the filter is type level, then the filter is an array, each item of which makes a LevelFilter
            else if (filter["type"] == "level") {
                if (!filter["value"].is_array()) {
                    logger->Warning("FlowMeasureFilterParser::ParseLevel: Json is not an array");
                    return nullptr;
                }

                // TODO: Make a level filter that accepts a number of levels
                for (const auto& level: filter["value"]) {
                    auto levelFilter = CreateLevelFilter(level, FlowMeasure::LevelFilterType::At);
                    if (!levelFilter) {
                        logger->Warning("FlowMeasureFilterParser::Parse: Could not create level filter");
                        return nullptr;
                    }

                    levelFilters.push_back(std::move(levelFilter));
                }
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

            // TODO: Distance from destination filter

            logger->Warning("FlowMeasureFilterParser::Parse: Unknown filter type");
        }

        return std::make_unique<FlowMeasure::ConcreteFlowMeasureFilters>(
                std::move(airportFilters), std::move(eventFilters), std::move(routeFilters), std::move(levelFilters)
        );
    }

    auto FlowMeasureFilterParser::CreateAirportFilter(const nlohmann::json& data) const
            -> std::shared_ptr<FlowMeasure::AirportFilter>
    {
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

        return std::make_shared<FlowMeasure::ConcreteAirportFilter>(airports);
    }

    auto FlowMeasureFilterParser::CreateLevelFilter(const nlohmann::json& data, FlowMeasure::LevelFilterType type) const
            -> std::shared_ptr<FlowMeasure::LevelFilter>
    {
        if (!data.is_number_integer()) {
            logger->Warning("FlowMeasureFilterParser::CreateLevelFilter: Json value is not an integer");
            return nullptr;
        }

        return std::make_shared<FlowMeasure::ConcreteLevelFilter>(type, data.get<int>());
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
}// namespace FlowSdk::Api
