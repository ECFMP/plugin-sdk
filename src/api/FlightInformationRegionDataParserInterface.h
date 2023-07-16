#pragma once
#include "InternalElementCollectionTypes.h"
#include "nlohmann/json_fwd.hpp"

namespace ECFMP::Api {

    class FlightInformationRegionDataParserInterface
    {
        public:
        virtual ~FlightInformationRegionDataParserInterface() = default;
        [[nodiscard]] virtual auto ParseFirs(const nlohmann::json& data)
                -> std::shared_ptr<InternalFlightInformationRegionCollection> = 0;
    };
}// namespace ECFMP::Api
