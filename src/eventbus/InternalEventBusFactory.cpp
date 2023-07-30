#include "eventbus/InternalEventBusFactory.h"
#include "eventbus/PendingEuroscopeEvents.h"
#include "plugin/InternalSdkEvents.h"

namespace ECFMP::EventBus {
    [[nodiscard]] auto MakeEventBus() -> std::shared_ptr<InternalEventBus>
    {
        auto pendingEuroscopeEvents = std::make_shared<PendingEuroscopeEvents>();
        auto eventDispatcherFactory = std::make_shared<EventDispatcherFactory>(pendingEuroscopeEvents);

        auto eventBus = std::make_shared<InternalEventBus>(eventDispatcherFactory);
        eventBus->SubscribeSync<Plugin::EuroscopeTimerTickEvent>(pendingEuroscopeEvents);

        return eventBus;
    }
}// namespace ECFMP::EventBus
