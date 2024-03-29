#pragma once
#include "ECFMP/api/ElementCollectionTypes.h"
#include "FlightInformationRegionDataParserInterface.h"
#include "InternalElementCollectionTypes.h"
#include "nlohmann/json_fwd.hpp"

namespace ECFMP {
    namespace EventBus {
        class InternalEventBus;
    }// namespace EventBus
    namespace Log {
        class Logger;
    }// namespace Log
}// namespace ECFMP

namespace ECFMP::Api {
    /**
     * Parses and updates flight information regions.
     */
    class FlightInformationRegionDataParser : public FlightInformationRegionDataParserInterface
    {
        public:
        FlightInformationRegionDataParser(
                std::shared_ptr<Log::Logger> logger, std::shared_ptr<EventBus::InternalEventBus> eventBus
        );
        [[nodiscard]] auto ParseFirs(const nlohmann::json& data)
                -> std::shared_ptr<InternalFlightInformationRegionCollection> override;

        private:
        [[nodiscard]] static auto DataIsValid(const nlohmann::json& data) -> bool;
        [[nodiscard]] static auto FirDataIsValid(const nlohmann::json& data) -> bool;

        // A logger, for logging things
        std::shared_ptr<Log::Logger> logger;

        // EventBus
        std::shared_ptr<EventBus::InternalEventBus> eventBus;
    };
}// namespace ECFMP::Api
