#pragma once
#include "FlowMeasureMeasureParserInterface.h"
#include "flow-sdk/Measure.h"
#include "nlohmann/json_fwd.hpp"

namespace FlowSdk::Log {
    class Logger;
}// namespace FlowSdk::Log

namespace FlowSdk::Api {
    class FlowMeasureMeasureParser : public FlowMeasureMeasureParserInterface
    {
        public:
        FlowMeasureMeasureParser(std::shared_ptr<Log::Logger> logger);
        [[nodiscard]] auto Parse(const nlohmann::json& data) const -> std::unique_ptr<FlowMeasure::Measure> override;

        private:
        [[nodiscard]] static auto GetType(const std::string& type) -> FlowMeasure::MeasureType;

        // Logger
        std::shared_ptr<Log::Logger> logger;
    };
}// namespace FlowSdk::Api
