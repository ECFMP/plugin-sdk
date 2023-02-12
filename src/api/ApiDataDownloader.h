#pragma once
#include "nlohmann/json_fwd.hpp"

namespace FlowSdk {
    namespace Http {
        class HttpClient;
    }// namespace Http
    namespace Log {
        class Logger;
    }// namespace Log
    namespace Plugin {
        template<class... Types>
        class InternalEventListeners;
    }// namespace Plugin
}// namespace FlowSdk

namespace FlowSdk::Api {

    /**
     * Downloads API data and disseminates it to any listeners.
     */
    class ApiDataDownloader
    {
        public:
        explicit ApiDataDownloader(std::unique_ptr<Http::HttpClient> httpClient,
                                   std::unique_ptr<Plugin::InternalEventListeners<const nlohmann::json&>> listeners,
                                   std::shared_ptr<Log::Logger> logger);

        ~ApiDataDownloader();
        void DownloadData();

        private:
        // HTTP client for getting data
        std::unique_ptr<Http::HttpClient> httpClient;

        // All the listeners
        std::unique_ptr<Plugin::InternalEventListeners<const nlohmann::json&>> listeners;

        // Logger
        std::shared_ptr<Log::Logger> logger;
    };
}// namespace FlowSdk::Api
