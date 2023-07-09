#pragma once
#include "../src/eventbus/InternalEventStream.h"
#include <any>
#include <mutex>
#include <typeindex>
#include <unordered_map>

namespace FlowSdk::EventBus {

    template<typename EventType>
    class EventStreamFactory;

    class EventBus
    {
        public:
        virtual ~EventBus() = default;
        /**
         * Subscribes the given listener to the event stream.
         */
        template<typename EventType>
        void Subscribe(std::shared_ptr<NewEventListener<EventType>> listener)
        {
            Subscribe<EventType>(listener, nullptr);
        };

        template<typename EventType>
        void Subscribe(
                std::shared_ptr<NewEventListener<EventType>> listener, std::shared_ptr<NewEventFilter<EventType>> filter
        )
        {
            GetStream<EventType>().Subscribe(listener, filter);
        };

        /**
         * Subscribes the given listener to the event stream, but only for the next event.
         */
        template<typename EventType>
        void SubscribeOnce(std::shared_ptr<NewEventListener<EventType>> listener)
        {
            SubscribeOnce<EventType>(listener, nullptr);
        }

        template<typename EventType>
        void SubscribeOnce(
                std::shared_ptr<NewEventListener<EventType>> listener, std::shared_ptr<NewEventFilter<EventType>> filter
        )
        {
            GetStream<EventType>().SubscribeOnce(listener, filter);
        };

        private:
        template<typename EventType>
        auto GetStream() -> InternalEventStream<EventType>&
        {
            auto lock = std::lock_guard(mutex);

            const auto index = std::type_index(typeid(EventType));
            if (!streams.contains(index)) {
                streams.insert({index, std::any(std::make_shared<InternalEventStream<EventType>>())});
            }

            return *std::any_cast<std::shared_ptr<InternalEventStream<EventType>>>(streams.at(index));
        }

        // Protects the streams map
        std::mutex mutex;

        // All the streams
        std::unordered_map<std::type_index, std::any> streams;
    };
}// namespace FlowSdk::EventBus
