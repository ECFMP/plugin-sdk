#pragma once
#include "InternalElementCollectionTypes.h"
#include "ECFMP/ElementCollectionTypes.h"
#include "nlohmann/json_fwd.hpp"

namespace ECFMP {
    namespace FlowMeasure {
        class FlowMeasureFilters;
    }// namespace FlowMeasure
    namespace Log {
        class Logger;
    }// namespace Log
}// namespace ECFMP

namespace ECFMP::Api {
    class FlowMeasureFilterParserInterface
    {
        public:
        virtual ~FlowMeasureFilterParserInterface() = default;
        [[nodiscard]] virtual auto Parse(const nlohmann::json& data, const InternalEventCollection& events) const
                -> std::unique_ptr<FlowMeasure::FlowMeasureFilters> = 0;
    };
}// namespace ECFMP::Api
