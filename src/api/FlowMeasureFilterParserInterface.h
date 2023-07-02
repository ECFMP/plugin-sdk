#pragma once
#include "InternalElementCollectionTypes.h"
#include "flow-sdk/ElementCollectionTypes.h"
#include "nlohmann/json_fwd.hpp"

namespace FlowSdk {
    namespace FlowMeasure {
        class FlowMeasureFilters;
    }// namespace FlowMeasure
    namespace Log {
        class Logger;
    }// namespace Log
}// namespace FlowSdk

namespace FlowSdk::Api {
    class FlowMeasureFilterParserInterface
    {
        public:
        virtual ~FlowMeasureFilterParserInterface() = default;
        [[nodiscard]] virtual auto Parse(const nlohmann::json& data, const InternalEventCollection& events) const
                -> std::unique_ptr<FlowMeasure::FlowMeasureFilters> = 0;
    };
}// namespace FlowSdk::Api
