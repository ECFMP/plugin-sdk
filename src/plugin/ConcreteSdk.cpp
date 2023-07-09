#include "ConcreteSdk.h"
#include "eventbus/InternalEventBus.h"

namespace FlowSdk::Plugin {
    ConcreteSdk::ConcreteSdk(std::shared_ptr<void> apiScheduler, std::shared_ptr<EventBus::InternalEventBus> eventBus)
        : apiScheduler(std::move(apiScheduler)), eventBus(std::move(eventBus))
    {
        assert(this->apiScheduler && "Api scheduler not set in ConcreteSdk");
        assert(this->eventBus && "Event bus not set in ConcreteSdk");
    }

    void ConcreteSdk::Destroy()
    {
        // Shutdown the API data downloader
        apiScheduler.reset();
    }

    auto ConcreteSdk::EventBus() const noexcept -> EventBus::EventBus&
    {
        return *eventBus;
    }

    void ConcreteSdk::OnEuroscopeTimerTick()
    {
        eventBus->ProcessPendingEvents();
    }
}// namespace FlowSdk::Plugin
