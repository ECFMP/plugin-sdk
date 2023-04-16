#pragma once
#include "ApiDataListenerTypes.h"
#include "InternalElementCollectionTypes.h"
#include "nlohmann/json_fwd.hpp"

namespace FlowSdk {
    namespace Log {
        class Logger;
    }// namespace Log
}// namespace FlowSdk

namespace FlowSdk::Api {
    class EventDataParser : public ApiDataListener
    {
        public:
        EventDataParser(
                std::shared_ptr<InternalEventCollection> events,
                std::shared_ptr<const InternalFlightInformationRegionCollection> firs,
                std::shared_ptr<Log::Logger> logger
        );
        void OnEvent(const nlohmann::json& data) override;

        private:
        [[nodiscard]] static auto DataIsValid(const nlohmann::json& data) -> bool;
        [[nodiscard]] auto EventDataIsValid(const nlohmann::json& data) -> bool;
        [[nodiscard]] static auto DateValid(const nlohmann::json& data, const std::string& key) -> bool;
        [[nodiscard]] static auto ParticipantsValid(const nlohmann::json& data) -> bool;

        // A logger, for logging things
        std::shared_ptr<Log::Logger> logger;

        // Stores the event objects
        std::shared_ptr<InternalEventCollection> events;

        // Stores firs
        std::shared_ptr<const InternalFlightInformationRegionCollection> firs;
    };
}// namespace FlowSdk::Api
