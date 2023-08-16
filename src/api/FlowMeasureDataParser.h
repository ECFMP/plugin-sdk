#pragma once
#include "ECFMP/flowmeasure/FlowMeasure.h"
#include "FlowMeasureDataParserInterface.h"
#include "eventbus/InternalEventBus.h"
#include "nlohmann/json_fwd.hpp"

namespace ECFMP {
    namespace FlowMeasure {
        class CustomFlowMeasureFilter;
    }// namespace FlowMeasure
    namespace Log {
        class Logger;
    }// namespace Log
}// namespace ECFMP

namespace ECFMP::Api {

    class FlowMeasureFilterParserInterface;
    class FlowMeasureMeasureParserInterface;

    class FlowMeasureDataParser : public FlowMeasureDataParserInterface
    {
        public:
        FlowMeasureDataParser(
                std::unique_ptr<FlowMeasureFilterParserInterface> filterParser,
                std::unique_ptr<FlowMeasureMeasureParserInterface> measureParser, std::shared_ptr<Log::Logger> logger,
                std::shared_ptr<EventBus::InternalEventBus> eventBus,
                std::shared_ptr<std::vector<std::shared_ptr<ECFMP::FlowMeasure::CustomFlowMeasureFilter>>> customFilters
        );
        ~FlowMeasureDataParser() override = default;
        [[nodiscard]] auto ParseFlowMeasures(
                const nlohmann::json& data, const InternalEventCollection& events,
                const InternalFlightInformationRegionCollection& firs
        ) -> std::shared_ptr<InternalFlowMeasureCollection> override;

        private:
        [[nodiscard]] static auto DataIsValid(const nlohmann::json& data) -> bool;
        [[nodiscard]] static auto FlowMeasurePropertiesValid(const nlohmann::json& data) -> bool;
        [[nodiscard]] auto
        GetNotifiedFirs(const nlohmann::json& data, const InternalFlightInformationRegionCollection& firs) const
                -> std::vector<std::shared_ptr<const ECFMP::FlightInformationRegion::FlightInformationRegion>>;
        [[nodiscard]] static auto GetMeasureStatus(
                const std::chrono::system_clock::time_point& startTime,
                const std::chrono::system_clock::time_point& endTime,
                const std::chrono::system_clock::time_point& withdrawnTime
        ) -> FlowMeasure::MeasureStatus;

        // Filter parser
        std::unique_ptr<FlowMeasureFilterParserInterface> filterParser;

        // Measure parser
        std::unique_ptr<FlowMeasureMeasureParserInterface> measureParser;

        // Logger
        std::shared_ptr<Log::Logger> logger;

        // Event bus
        std::shared_ptr<EventBus::InternalEventBus> eventBus;

        // Custom flow measure filters
        std::shared_ptr<std::vector<std::shared_ptr<ECFMP::FlowMeasure::CustomFlowMeasureFilter>>> customFilters;
    };

}// namespace ECFMP::Api
