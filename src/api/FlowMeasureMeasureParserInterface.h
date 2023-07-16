#pragma once
#include "InternalElementCollectionTypes.h"
#include "nlohmann/json_fwd.hpp"

namespace ECFMP {
    namespace FlowMeasure {
        class Measure;
    }// namespace FlowMeasure
    namespace Log {
        class Logger;
    }// namespace Log
}// namespace ECFMP

namespace ECFMP::Api {
    class FlowMeasureMeasureParserInterface
    {
        public:
        virtual ~FlowMeasureMeasureParserInterface() = default;
        [[nodiscard]] virtual auto Parse(const nlohmann::json& data) const -> std::unique_ptr<FlowMeasure::Measure> = 0;
    };
}// namespace ECFMP::Api
