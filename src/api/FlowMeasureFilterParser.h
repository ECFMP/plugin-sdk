#pragma once
#include "FlowMeasureFilterParserInterface.h"
#include "flow-sdk/FlowMeasureFilters.h"
#include "flow-sdk/LevelFilter.h"

namespace FlowSdk {
    namespace FlowMeasure {
        class AirportFilter;
        class EventFilter;
        class RouteFilter;
    }// namespace FlowMeasure
    namespace Log {
        class Logger;
    }// namespace Log
}// namespace FlowSdk

namespace FlowSdk::Api {
    class FlowMeasureFilterParser : public FlowMeasureFilterParserInterface
    {
        public:
        explicit FlowMeasureFilterParser(const std::shared_ptr<FlowSdk::Log::Logger>& logger);
        [[nodiscard]] auto Parse(const nlohmann::json& data, const InternalEventCollection& events) const
                -> std::unique_ptr<FlowMeasure::FlowMeasureFilters> override;

        private:
        [[nodiscard]] auto CreateAirportFilter(const nlohmann::json& data, const std::string& type) const
                -> std::shared_ptr<FlowMeasure::AirportFilter>;

        [[nodiscard]] auto CreateLevelFilter(const nlohmann::json& data, FlowMeasure::LevelFilterType type) const
                -> std::shared_ptr<FlowMeasure::LevelFilter>;

        [[nodiscard]] auto CreateEventFilter(const nlohmann::json& data, const InternalEventCollection& events) const
                -> std::shared_ptr<FlowMeasure::EventFilter>;

        [[nodiscard]] auto CreateRouteFilter(const nlohmann::json& data) const
                -> std::shared_ptr<FlowMeasure::RouteFilter>;

        // Logger
        std::shared_ptr<FlowSdk::Log::Logger> logger;
    };
}// namespace FlowSdk::Api
