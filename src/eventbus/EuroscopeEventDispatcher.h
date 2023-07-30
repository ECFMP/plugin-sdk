#pragma once
#include "ECFMP/eventbus/EventListener.h"
#include "EventDispatcher.h"
#include "PendingEuroscopeEvents.h"
#include <cassert>

namespace ECFMP::EventBus {

    template<typename EventType>
    class EuroscopeEventDispatcher : public EventDispatcher<EventType>
    {
        public:
        explicit EuroscopeEventDispatcher(
                std::shared_ptr<EventListener<EventType>> listener,
                const std::shared_ptr<PendingEuroscopeEvents>& pendingEuroscopeEvents
        )
            : listener(listener), pendingEuroscopeEvents(pendingEuroscopeEvents)
        {
            assert(listener != nullptr && "Listener cannot be null");
            assert(pendingEuroscopeEvents != nullptr && "Pending ES events cannot be null");
        }

        void Dispatch(const EventType& event) override
        {
            // Copy the listener to make sure it doesn't get deleted before the event is dispatched
            auto listenerCopy = this->listener;
            pendingEuroscopeEvents->AddEvent([listenerCopy, event]() -> void {
                listenerCopy->OnEvent(event);
            });
        };

        private:
        // The event listener for this dispatcher
        std::shared_ptr<EventListener<EventType>> listener;

        // The pending ES events
        std::shared_ptr<PendingEuroscopeEvents> pendingEuroscopeEvents;
    };
}// namespace ECFMP::EventBus
