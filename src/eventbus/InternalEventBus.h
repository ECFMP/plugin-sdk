#pragma once
#include "ECFMP/eventbus/EventBus.h"

namespace ECFMP::EventBus {
    class InternalEventBus : public EventBus
    {
        public:
        explicit InternalEventBus(const std::shared_ptr<EventDispatcherFactory>& dispatcherFactory)
            : EventBus(dispatcherFactory)
        {}

        template<typename EventType>
        void OnEvent(const EventType& event)
        {
            GetStream<EventType>().OnEvent(event);
        }

        /**
         * Subscribes the given listener to the event stream, asynchronously.
         */
        template<typename EventType>
        void SubscribeAsync(std::shared_ptr<EventListener<EventType>> listener)
        {
            SubscribeAsync<EventType>(listener, nullptr);
        };

        template<typename EventType>
        void SubscribeAsync(
                std::shared_ptr<EventListener<EventType>> listener, std::shared_ptr<EventFilter<EventType>> filter
        )
        {
            if (listener == nullptr) {
                throw std::invalid_argument("Listener cannot be null");
            }

            Subscribe<EventType>(EventSubscription{
                    dispatcherFactory->CreateDispatcher<EventType>(listener, EventDispatchMode::Async),
                    listener,
                    filter,
                    {EventDispatchMode::Async, false}});
        };

        /**
         * Subscribes the given listener to the event stream, asynchronously, once.
         */
        template<typename EventType>
        void SubscribeAsyncOnce(std::shared_ptr<EventListener<EventType>> listener)
        {
            SubscribeAsyncOnce<EventType>(listener, nullptr);
        };

        template<typename EventType>
        void SubscribeAsyncOnce(
                std::shared_ptr<EventListener<EventType>> listener, std::shared_ptr<EventFilter<EventType>> filter
        )
        {
            if (listener == nullptr) {
                throw std::invalid_argument("Listener cannot be null");
            }

            Subscribe<EventType>(EventSubscription{
                    dispatcherFactory->CreateDispatcher<EventType>(listener, EventDispatchMode::Async),
                    listener,
                    filter,
                    {EventDispatchMode::Async, true}});
        };

        /**
         * Subscribes the given listener to the event stream, synchronously.
         */
        template<typename EventType>
        void SubscribeSync(std::shared_ptr<EventListener<EventType>> listener)
        {
            SubscribeSync<EventType>(listener, nullptr);
        };

        template<typename EventType>
        void SubscribeSync(
                std::shared_ptr<EventListener<EventType>> listener, std::shared_ptr<EventFilter<EventType>> filter
        )
        {
            if (listener == nullptr) {
                throw std::invalid_argument("Listener cannot be null");
            }

            Subscribe<EventType>(EventSubscription{
                    dispatcherFactory->CreateDispatcher<EventType>(listener, EventDispatchMode::Sync),
                    listener,
                    filter,
                    {EventDispatchMode::Sync, false}});
        };

        /**
         * Subscribes the given listener to the event stream, once, synchronously.
         */
        template<typename EventType>
        void SubscribeSyncOnce(std::shared_ptr<EventListener<EventType>> listener)
        {
            SubscribeSyncOnce<EventType>(listener, nullptr);
        };

        template<typename EventType>
        void SubscribeSyncOnce(
                std::shared_ptr<EventListener<EventType>> listener, std::shared_ptr<EventFilter<EventType>> filter
        )
        {
            if (listener == nullptr) {
                throw std::invalid_argument("Listener cannot be null");
            }

            Subscribe<EventType>(EventSubscription{
                    dispatcherFactory->CreateDispatcher<EventType>(listener, EventDispatchMode::Sync),
                    listener,
                    filter,
                    {EventDispatchMode::Sync, true}});
        };

        /**
         * Checks whether a specified type of listener is registered for a specified type of event.
         *
         * Can be used for test assertions.
         */
        template<typename ListenerType, typename EventType>
        [[nodiscard]] auto HasListenerForSubscription(const SubscriptionFlags& flags) -> bool
        {
            return GetStream<EventType>().template HasListenerForSubscription<ListenerType>(flags);
        }
    };
}// namespace ECFMP::EventBus
