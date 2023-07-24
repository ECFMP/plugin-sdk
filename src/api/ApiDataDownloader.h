#pragma once
#include "ECFMP/eventbus/EventListener.h"
#include "eventbus/InternalEventBus.h"
#include "nlohmann/json_fwd.hpp"
#include "plugin/InternalSdkEvents.h"

namespace ECFMP {
    namespace EventBus {
        class InternalEventBus;
    }// namespace EventBus
    namespace Http {
        class HttpClient;
    }// namespace Http
    namespace Log {
        class Logger;
    }// namespace Log
}// namespace ECFMP

namespace ECFMP::Api {

    /**
     * Downloads API data and disseminates it to any listeners.
     */
    class ApiDataDownloader : public EventBus::EventListener<Plugin::ApiDataDownloadRequiredEvent>
    {
        public:
        explicit ApiDataDownloader(
                std::unique_ptr<Http::HttpClient> httpClient, std::shared_ptr<EventBus::InternalEventBus> eventBus,
                std::shared_ptr<Log::Logger> logger
        );

        ~ApiDataDownloader();
        void OnEvent(const Plugin::ApiDataDownloadRequiredEvent& event) override;

        private:
        // HTTP client for getting data
        std::unique_ptr<Http::HttpClient> httpClient;

        // Eventbus
        std::shared_ptr<EventBus::InternalEventBus> eventBus;

        // Logger
        std::shared_ptr<Log::Logger> logger;
    };
}// namespace ECFMP::Api
