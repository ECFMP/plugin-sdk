#pragma once
#include "flow-sdk/EventListener.h"
#include "nlohmann/json_fwd.hpp"

namespace FlowSdk {
    namespace FlightInformationRegion {
        class InternalFlightInformationRegionCollection;
    }// namespace FlightInformationRegion
    namespace Log {
        class Logger;
    }// namespace Log
}// namespace FlowSdk

namespace FlowSdk::Api {
    /**
     * Parses and updates flight information regions.
     */
    class FlightInformationRegionDataParser : public Plugin::EventListener<const nlohmann::json&>
    {
        public:
        FlightInformationRegionDataParser(
                std::shared_ptr<FlightInformationRegion::InternalFlightInformationRegionCollection> firs,
                std::shared_ptr<Log::Logger> logger
        );
        void OnEvent(const nlohmann::json& data) override;

        private:
        [[nodiscard]] static auto DataIsValid(const nlohmann::json& data) -> bool;
        [[nodiscard]] static auto FirDataIsValid(const nlohmann::json& data) -> bool;

        // A logger, for logging things
        std::shared_ptr<Log::Logger> logger;

        // Stores the FIR objects
        std::shared_ptr<FlightInformationRegion::InternalFlightInformationRegionCollection> firs;
    };
}// namespace FlowSdk::Api
