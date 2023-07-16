#pragma once
#include "InternalElementCollectionTypes.h"
#include "nlohmann/json_fwd.hpp"

namespace ECFMP::Api {

    class EventDataParserInterface
    {
        public:
        virtual ~EventDataParserInterface() = default;
        [[nodiscard]] virtual auto
        ParseEvents(const nlohmann::json& data, const InternalFlightInformationRegionCollection& firs)
                -> std::shared_ptr<InternalEventCollection> = 0;
    };
}// namespace ECFMP::Api
