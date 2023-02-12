#pragma once

namespace FlowSdk::Api {

    class ApiDataDownloader;

    /**
     * Responsible for downloading API data on a schedule.
     */
    class ApiDataScheduler
    {
        public:
        explicit ApiDataScheduler(std::unique_ptr<ApiDataDownloader> downloader);
        ~ApiDataScheduler();

        private:
        struct Impl;
        std::unique_ptr<Impl> impl;
    };

}// namespace FlowSdk::Api
