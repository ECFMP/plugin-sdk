#pragma once
#include <memory>
#include <mutex>
#include <vector>

namespace FlowSdk::EventBus {

    template<typename EventType>
    class NewEventListener;
    template<typename EventType>
    class NewEventFilter;

    template<typename EventType>
    struct EventSubscription {
        std::shared_ptr<NewEventListener<EventType>> listener;
        std::shared_ptr<NewEventFilter<EventType>> filter;
        bool once;
    };

    template<typename EventType>
    class EventStream
    {
        public:
        virtual ~EventStream() = default;

        /**
         * Subscribes the given listener to the event stream.
         */
        void Subscribe(std::shared_ptr<NewEventListener<EventType>> listener)
        {
            Subscribe(listener, nullptr);
        };

        virtual void Subscribe(
                std::shared_ptr<NewEventListener<EventType>> listener, std::shared_ptr<NewEventFilter<EventType>> filter
        )
        {
            if (listener == nullptr) {
                throw std::invalid_argument("listener cannot be null");
            }

            auto guard = std::lock_guard(mutex);
            subscriptions.push_back(EventSubscription<EventType>{listener, filter, false});
        };

        /**
         * Subscribes the given listener to the event stream, but only for the next event.
         */
        virtual void SubscribeOnce(std::shared_ptr<NewEventListener<EventType>> listener)
        {
            SubscribeOnce(listener, nullptr);
        }

        virtual void SubscribeOnce(
                std::shared_ptr<NewEventListener<EventType>> listener, std::shared_ptr<NewEventFilter<EventType>> filter
        )
        {
            if (listener == nullptr) {
                throw std::invalid_argument("listener cannot be null");
            }

            auto guard = std::lock_guard(mutex);
            subscriptions.push_back(EventSubscription<EventType>{listener, filter, true});
        };

        protected:
        std::mutex mutex;

        // All subscriptions to this event stream.
        std::vector<EventSubscription<EventType>> subscriptions;
    };
}// namespace FlowSdk::EventBus
