#pragma once
#include "InternalElementCollectionTypes.h"
#include "nlohmann/json_fwd.hpp"

namespace ECFMP::Api {

    class FlowMeasureDataParserInterface
    {
        public:
        virtual ~FlowMeasureDataParserInterface() = default;
        [[nodiscard]] virtual auto ParseFlowMeasures(
                const nlohmann::json& data, const InternalEventCollection& events,
                const InternalFlightInformationRegionCollection& firs
        ) -> std::shared_ptr<InternalFlowMeasureCollection> = 0;
    };
}// namespace ECFMP::Api
