#include "ApiDataScheduler.h"
#include "time/Clock.h"

namespace ECFMP::Api {

    struct ApiDataScheduler::Impl {

        explicit Impl(std::shared_ptr<EventBus::InternalEventBus> eventBus) : eventBus(std::move(eventBus))
        {}

        // The interval for downloading
        const std::chrono::seconds runInterval = std::chrono::seconds(90);

        // The last time we ran
        std::chrono::system_clock::time_point lastRuntime = (std::chrono::system_clock::time_point::min)();

        // For publishing events
        std::shared_ptr<EventBus::InternalEventBus> eventBus;
    };

    ApiDataScheduler::ApiDataScheduler(std::shared_ptr<EventBus::InternalEventBus> eventBus)
        : impl(std::make_unique<Impl>(std::move(eventBus)))
    {
        assert(impl->eventBus && "Event bus not set in ApiDataScheduler");
    }

    ApiDataScheduler::~ApiDataScheduler() = default;

    void ApiDataScheduler::OnEvent(const Plugin::EuroscopeTimerTickEvent& event)
    {
        const auto now = Time::TimeNow();
        if (impl->lastRuntime + impl->runInterval < now) {
            impl->lastRuntime = now;
            impl->eventBus->OnEvent<Plugin::ApiDataDownloadRequiredEvent>({});
        }
    }
}// namespace ECFMP::Api
