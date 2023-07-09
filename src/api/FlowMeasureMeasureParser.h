#pragma once
#include "FlowMeasureMeasureParserInterface.h"
#include "ECFMP/Measure.h"
#include "nlohmann/json_fwd.hpp"

namespace ECFMP::Log {
    class Logger;
}// namespace ECFMP::Log

namespace ECFMP::Api {
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
}// namespace ECFMP::Api
