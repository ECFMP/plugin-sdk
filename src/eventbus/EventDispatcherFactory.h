#pragma once
#include "AsynchronousEventDispatcher.h"
#include "EuroscopeEventDispatcher.h"
#include "EventDispatcher.h"
#include "PendingEuroscopeEvents.h"
#include "SubscriptionFlags.h"
#include "SynchronousEventDispatcher.h"
#include <assert.h>

namespace ECFMP::EventBus {
    class EventDispatcherFactory
    {
        public:
        explicit EventDispatcherFactory(const std::shared_ptr<PendingEuroscopeEvents>& pendingEuroscopeEvents)
            : pendingEuroscopeEvents(pendingEuroscopeEvents)
        {
            assert(pendingEuroscopeEvents != nullptr && "pendingEuroscopeEvents cannot be null");
        }
        virtual ~EventDispatcherFactory() = default;

        template<typename EventType>
        [[nodiscard]] auto CreateDispatcher(std::shared_ptr<EventListener<EventType>> listener, EventDispatchMode mode)
                -> std::shared_ptr<EventDispatcher<EventType>>
        {
            switch (mode) {
            case EventDispatchMode::Sync:
                return std::make_shared<SynchronousEventDispatcher<EventType>>(listener);
            case EventDispatchMode::Async:
                return std::make_shared<AsynchronousEventDispatcher<EventType>>(listener);
            case EventDispatchMode::Euroscope:
                return std::make_shared<EuroscopeEventDispatcher<EventType>>(listener, pendingEuroscopeEvents);
            default:
                throw std::runtime_error("Unknown dispatch mode");
            }
        };

        private:
        // A place that stores events that should be dispatched on the Euroscope thread.
        std::shared_ptr<PendingEuroscopeEvents> pendingEuroscopeEvents;
    };
}// namespace ECFMP::EventBus
