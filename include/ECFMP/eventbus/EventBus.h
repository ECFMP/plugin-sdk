#pragma once
#include "../../src/eventbus/EventDispatcherFactory.h"
#include "../../src/eventbus/EventStream.h"
#include "../../src/eventbus/SubscriptionFlags.h"
#include <any>
#include <mutex>
#include <typeindex>
#include <unordered_map>

namespace ECFMP::EventBus {

    class EventBus
    {
        public:
        explicit EventBus(const std::shared_ptr<EventDispatcherFactory>& dispatcherFactory)
            : dispatcherFactory(dispatcherFactory)
        {
            assert(dispatcherFactory != nullptr && "Dispatcher factory cannot be null");
        }

        virtual ~EventBus() = default;
        /**
         * Subscribes the given listener to the event stream.
         */
        template<typename EventType>
        void Subscribe(std::shared_ptr<EventListener<EventType>> listener)
        {
            Subscribe<EventType>(listener, nullptr);
        };

        template<typename EventType>
        void
        Subscribe(std::shared_ptr<EventListener<EventType>> listener, std::shared_ptr<EventFilter<EventType>> filter)
        {
            if (listener == nullptr) {
                throw std::invalid_argument("Listener cannot be null");
            }

            Subscribe<EventType>(EventSubscription{
                    dispatcherFactory->CreateDispatcher<EventType>(listener, EventDispatchMode::Euroscope),
                    listener,
                    filter,
                    {EventDispatchMode::Euroscope, false}});
        };

        /**
         * Subscribes the given listener to the event stream, but only for the next event.
         */
        template<typename EventType>
        void SubscribeOnce(std::shared_ptr<EventListener<EventType>> listener)
        {
            SubscribeOnce<EventType>(listener, nullptr);
        }

        template<typename EventType>
        void SubscribeOnce(
                std::shared_ptr<EventListener<EventType>> listener, std::shared_ptr<EventFilter<EventType>> filter
        )
        {
            if (listener == nullptr) {
                throw std::invalid_argument("Listener cannot be null");
            }

            Subscribe<EventType>(EventSubscription{
                    dispatcherFactory->CreateDispatcher<EventType>(listener, EventDispatchMode::Euroscope),
                    listener,
                    filter,
                    {EventDispatchMode::Euroscope, true}});
        };

        /**
         * Checks whether a specified type of listener is registered for a specified type of event.
         *
         * Can be used for test assertions.
         */
        template<typename ListenerType, typename EventType>
        [[nodiscard]] auto HasListenerOfType() -> bool
        {
            return GetStream<EventType>().template HasListenerOfType<ListenerType>();
        }

        protected:
        template<typename ListenerType, typename EventType>
        void Subscribe(const EventSubscription<EventType>& subscription)
        {
            GetStream<EventType>().Subscribe(subscription);
        }

        // Factory for dispatchers
        std::shared_ptr<EventDispatcherFactory> dispatcherFactory;

        private:
        template<typename EventType>
        auto GetStream() -> EventStream<EventType>&
        {
            auto lock = std::lock_guard(mutex);

            const auto index = std::type_index(typeid(EventType));
            if (!streams.contains(index)) {
                streams.insert({index, std::any(std::make_shared<EventStream<EventType>>())});
            }

            return *std::any_cast<std::shared_ptr<EventStream<EventType>>>(streams.at(index));
        }

        // Protects the streams map
        std::mutex mutex;

        // All the streams
        std::unordered_map<std::type_index, std::any> streams;
    };
}// namespace ECFMP::EventBus
