#pragma once
#include "ApiDataListenerTypes.h"
#include "InternalElementCollectionTypes.h"
#include "nlohmann/json_fwd.hpp"

namespace ECFMP {
    namespace Log {
        class Logger;
    }// namespace Log
}// namespace ECFMP

namespace ECFMP::Api {
    /**
     * Parses and updates flight information regions.
     */
    class FlightInformationRegionDataParser : public ApiDataListener
    {
        public:
        FlightInformationRegionDataParser(
                std::shared_ptr<InternalFlightInformationRegionCollection> firs, std::shared_ptr<Log::Logger> logger
        );
        void OnEvent(const nlohmann::json& data) override;

        private:
        [[nodiscard]] static auto DataIsValid(const nlohmann::json& data) -> bool;
        [[nodiscard]] static auto FirDataIsValid(const nlohmann::json& data) -> bool;

        // A logger, for logging things
        std::shared_ptr<Log::Logger> logger;

        // Stores the FIR objects
        std::shared_ptr<InternalFlightInformationRegionCollection> firs;
    };
}// namespace ECFMP::Api
