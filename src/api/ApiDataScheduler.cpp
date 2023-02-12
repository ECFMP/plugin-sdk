#include "ApiDataScheduler.h"
#include "ApiDataDownloader.h"
#include <chrono>
#include <condition_variable>
#include <mutex>
#include <thread>

namespace FlowSdk::Api {

    struct ApiDataScheduler::Impl {

        explicit Impl(std::unique_ptr<ApiDataDownloader> downloader)
            : downloader(std::move(downloader)), schedulerThread(std::make_unique<std::thread>(&Impl::Scheduler, this))
        {}

        void Scheduler()
        {
            std::unique_lock lock(conditionMutex);
            std::chrono::system_clock::time_point lastRuntime = std::chrono::system_clock::time_point::min();
            while (true) {
                // Wait until the next runtime, or until we are interrupted
                conditionVariable.wait_until(lock, lastRuntime + runInterval, [this]() {
                    return !running;
                });

                // Check we are still running
                if (!running) {
                    break;
                }

                // Do a download
                downloader->DownloadData();
                lastRuntime = std::chrono::system_clock::now();
            }
        }

        /**
         * Notify the thread that we're done, and stop.
         */
        void Stop()
        {
            std::unique_lock lock(conditionMutex);
            running = false;
            lock.unlock();

            conditionVariable.notify_one();
            schedulerThread->join();
        }

        // The interval for downloading
        const std::chrono::seconds runInterval = std::chrono::seconds(90);

        // Are we running
        bool running = true;

        // Used for waiting the thread when required
        std::condition_variable conditionVariable;

        // Used to grant access to the condition variable
        std::mutex conditionMutex;

        // Does the scheduling
        std::unique_ptr<std::thread> schedulerThread;

        // For downloading data.
        std::unique_ptr<ApiDataDownloader> downloader;
    };

    ApiDataScheduler::ApiDataScheduler(std::unique_ptr<ApiDataDownloader> downloader)
        : impl(std::make_unique<Impl>(std::move(downloader)))
    {
        assert(impl->downloader && "Downloader not set in ApiDataScheduler");
    }

    ApiDataScheduler::~ApiDataScheduler()
    {
        impl->Stop();
    };
}// namespace FlowSdk::Api
