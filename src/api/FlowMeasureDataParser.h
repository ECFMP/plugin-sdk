#pragma once
#include "ApiDataListenerTypes.h"
#include "InternalElementCollectionTypes.h"
#include "flow-sdk/FlowMeasure.h"
#include "nlohmann/json_fwd.hpp"
#include <memory>

namespace FlowSdk::Log {
    class Logger;
}// namespace FlowSdk::Log

namespace FlowSdk::Api {

    class FlowMeasureFilterParserInterface;
    class FlowMeasureMeasureParserInterface;

    class FlowMeasureDataParser : public ApiDataListener
    {
        public:
        FlowMeasureDataParser(
                std::unique_ptr<FlowMeasureFilterParserInterface> filterParser,
                std::unique_ptr<FlowMeasureMeasureParserInterface> measureParser,
                std::shared_ptr<InternalFlowMeasureCollection> flowMeasures,
                std::shared_ptr<InternalEventCollection> events,
                std::shared_ptr<const InternalFlightInformationRegionCollection> firs,
                std::shared_ptr<Log::Logger> logger
        );
        void OnEvent(const nlohmann::json& data) override;

        private:
        [[nodiscard]] static auto DataIsValid(const nlohmann::json& data) -> bool;
        [[nodiscard]] static auto FlowMeasurePropertiesValid(const nlohmann::json& data) -> bool;
        [[nodiscard]] auto GetNotifiedFirs(const nlohmann::json& data) const
                -> std::vector<std::shared_ptr<const FlowSdk::FlightInformationRegion::FlightInformationRegion>>;
        [[nodiscard]] auto GetMeasureStatus(
                const std::chrono::system_clock::time_point& startTime,
                const std::chrono::system_clock::time_point& endTime,
                const std::chrono::system_clock::time_point& withdrawnTime
        ) const -> FlowMeasure::MeasureStatus;

        // Filter parser
        std::unique_ptr<FlowMeasureFilterParserInterface> filterParser;

        // Measure parser
        std::unique_ptr<FlowMeasureMeasureParserInterface> measureParser;

        // All the flow measures
        std::shared_ptr<InternalFlowMeasureCollection> flowMeasures;

        // All the events
        std::shared_ptr<InternalEventCollection> events;

        // All the firs
        std::shared_ptr<const InternalFlightInformationRegionCollection> firs;

        // Logger
        std::shared_ptr<Log::Logger> logger;
    };

}// namespace FlowSdk::Api
