#pragma once
#include "ECFMP/eventbus/EventListener.h"
#include "EventDispatcher.h"
#include "thread/ThreadPool.h"
#include <cassert>
#include <future>

namespace ECFMP::EventBus {

    template<typename EventType>
    class AsynchronousEventDispatcher : public EventDispatcher<EventType>
    {
        public:
        explicit AsynchronousEventDispatcher(
                std::shared_ptr<EventListener<EventType>> listener, std::shared_ptr<Thread::ThreadPool> threadPool
        )
            : listener(listener), threadPool(threadPool)
        {
            assert(listener != nullptr && "Listener cannot be null");
            assert(threadPool != nullptr && "Thread pool cannot be null");
        }

        /**
         * Dispatch the event asynchronously.
         */
        void Dispatch(const EventType& event) override
        {
            // Copy the listener to ensure it is not destroyed before the event is dispatched
            auto listenerCopy = listener;
            auto eventCopy = event;

            // Dispatch the event asynchronously using the thread pool
            threadPool->Schedule([listenerCopy, eventCopy]() {
                listenerCopy->OnEvent(eventCopy);
            });
        };

        private:
        // The event listener for this dispatcher
        std::shared_ptr<EventListener<EventType>> listener;

        // The thread pool to use for dispatching events
        std::shared_ptr<Thread::ThreadPool> threadPool;
    };
}// namespace ECFMP::EventBus
