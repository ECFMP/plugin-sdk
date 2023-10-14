#include "eventbus/InternalEventBusFactory.h"
#include "eventbus/PendingEuroscopeEvents.h"
#include "plugin/InternalSdkEvents.h"
#include "thread/ThreadPool.h"

namespace ECFMP::EventBus {
    [[nodiscard]] auto MakeEventBus() -> std::shared_ptr<InternalEventBus>
    {
        // Thread pool will get stopped when the eventbus is destroyed
        auto threadPool = std::make_shared<Thread::ThreadPool>();
        auto pendingEuroscopeEvents = std::make_shared<PendingEuroscopeEvents>();
        auto eventDispatcherFactory = std::make_shared<EventDispatcherFactory>(pendingEuroscopeEvents, threadPool);

        auto eventBus = std::make_shared<InternalEventBus>(eventDispatcherFactory);
        eventBus->SubscribeSync<Plugin::EuroscopeTimerTickEvent>(pendingEuroscopeEvents);

        return eventBus;
    }
}// namespace ECFMP::EventBus
