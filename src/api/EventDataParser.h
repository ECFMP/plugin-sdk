#pragma once
#include "EventDataParserInterface.h"
#include "nlohmann/json_fwd.hpp"

namespace ECFMP::Log {
    class Logger;
}// namespace ECFMP::Log

namespace ECFMP::Api {
    class EventDataParser : public EventDataParserInterface
    {
        public:
        explicit EventDataParser(std::shared_ptr<Log::Logger> logger);
        [[nodiscard]] auto
        ParseEvents(const nlohmann::json& data, const InternalFlightInformationRegionCollection& firs)
                -> std::shared_ptr<InternalEventCollection> override;

        private:
        [[nodiscard]] static auto DataIsValid(const nlohmann::json& data) -> bool;
        [[nodiscard]] static auto
        EventDataIsValid(const nlohmann::json& data, const InternalFlightInformationRegionCollection& firs) -> bool;
        [[nodiscard]] static auto DateValid(const nlohmann::json& data, const std::string& key) -> bool;
        [[nodiscard]] static auto ParticipantsValid(const nlohmann::json& data) -> bool;

        // A logger, for logging things
        std::shared_ptr<Log::Logger> logger;
    };
}// namespace ECFMP::Api
