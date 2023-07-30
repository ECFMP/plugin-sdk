#pragma once
#include "ECFMP/eventbus/EventListener.h"
#include "EventDispatcher.h"
#include <assert.h>

namespace ECFMP::EventBus {

    template<typename EventType>
    class SynchronousEventDispatcher : public EventDispatcher<EventType>
    {
        public:
        explicit SynchronousEventDispatcher(std::shared_ptr<EventListener<EventType>> listener) : listener(listener)
        {
            assert(listener != nullptr && "Listener cannot be null");
        }

        void Dispatch(const EventType& event) override
        {
            listener->OnEvent(event);
        };

        private:
        // The event listener for this dispatcher
        std::shared_ptr<EventListener<EventType>> listener;
    };
}// namespace ECFMP::EventBus
