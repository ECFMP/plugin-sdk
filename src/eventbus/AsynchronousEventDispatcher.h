#pragma once
#include "ECFMP/eventbus/EventListener.h"
#include "EventDispatcher.h"
#include <cassert>
#include <future>

namespace ECFMP::EventBus {

    template<typename EventType>
    class AsynchronousEventDispatcher : public EventDispatcher<EventType>
    {
        public:
        explicit AsynchronousEventDispatcher(std::shared_ptr<EventListener<EventType>> listener) : listener(listener)
        {
            assert(listener != nullptr && "Listener cannot be null");
        }

        /**
         * Dispatch the event asynchronously.
         */
        void Dispatch(const EventType& event) override
        {
            // Copy the listener to ensure it is not destroyed before the event is dispatched
            auto listenerCopy = listener;
            static_cast<void>(std::async(
                    std::launch::async,
                    [listenerCopy](const auto& event) {
                        listenerCopy->OnEvent(event);
                    },
                    event
            ));
        };

        private:
        // The event listener for this dispatcher
        std::shared_ptr<EventListener<EventType>> listener;
    };
}// namespace ECFMP::EventBus
