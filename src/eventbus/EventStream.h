#pragma once
#include "ECFMP/eventbus/EventFilter.h"
#include "ECFMP/eventbus/EventListener.h"
#include "SubscriptionFlags.h"
#include "EventDispatcher.h"
#include <memory>
#include <mutex>
#include <utility>
#include <vector>

namespace ECFMP::EventBus {

    template<typename EventType>
    struct EventSubscription {

        EventSubscription(
                std::shared_ptr<EventDispatcher<EventType>> dispatcher,
                std::shared_ptr<EventListener<EventType>> rawListener, std::shared_ptr<EventFilter<EventType>> filter,
                SubscriptionFlags flags
        )
            : dispatcher(std::move(dispatcher)), rawListener(std::move(rawListener)), filter(std::move(filter)),
              flags(flags)
        {
            assert(this->dispatcher != nullptr && "Dispatcher cannot be null");
            assert(this->rawListener != nullptr && "Listener cannot be null");
        }

        std::shared_ptr<EventDispatcher<EventType>> dispatcher;
        std::shared_ptr<EventListener<EventType>> rawListener;
        std::shared_ptr<EventFilter<EventType>> filter;

        SubscriptionFlags flags;
    };

    template<typename EventType>
    class EventStream
    {
        public:
        virtual ~EventStream() = default;

        /**
         * Subscribes the given listener to the event stream.
         */
        void Subscribe(const EventSubscription<EventType>& subscription)
        {
            auto guard = std::lock_guard(mutex);
            subscriptions.push_back(subscription);
        };

        void OnEvent(const EventType& event)
        {
            auto guard = std::lock_guard(this->mutex);

            // Remove all subscriptions that are only valid for one event.
            this->subscriptions.erase(
                    std::remove_if(
                            this->subscriptions.begin(), this->subscriptions.end(),
                            [&event](const EventSubscription<EventType>& subscription) {
                                // If the subscription filter doesn't pass, then we don't want to call the listener
                                // but also don't want to remove the subscription.
                                if (subscription.filter != nullptr && !subscription.filter->ShouldProcess(event)) {
                                    return false;
                                }

                                // Dispatch the event to the subscription
                                subscription.dispatcher->Dispatch(event);

                                // For any called listeners that are only valid for one event, remove them.
                                return subscription.flags.once;
                            }
                    ),
                    this->subscriptions.end()
            );
        };

        /**
         * A method for testing only, shouldn't normally be used.
         */
        template<typename ListenerType>
        [[nodiscard]] auto HasListenerOfType() -> bool
        {
            auto guard = std::lock_guard(this->mutex);
            return std::any_of(
                    this->subscriptions.begin(), this->subscriptions.end(),
                    [](const EventSubscription<EventType>& subscription) {
                        try {
                            static_cast<void>(dynamic_cast<const ListenerType&>(*subscription.rawListener));
                            return true;
                        }
                        catch (std::bad_cast&) {
                            return false;
                        }
                    }
            );
        }

        /**
         * A method for testing only, shouldn't normally be used.
         */
        template<typename ListenerType>
        [[nodiscard]] auto HasListenerForSubscription(const SubscriptionFlags& expectedFlags) -> bool
        {
            auto guard = std::lock_guard(this->mutex);
            return std::any_of(
                    this->subscriptions.begin(), this->subscriptions.end(),
                    [&expectedFlags](const EventSubscription<EventType>& subscription) {
                        try {
                            static_cast<void>(dynamic_cast<const ListenerType&>(*subscription.rawListener));

                            return subscription.flags.once == expectedFlags.once
                                    && subscription.flags.dispatchMode == expectedFlags.dispatchMode;
                            return true;
                        }
                        catch (std::bad_cast&) {
                            return false;
                        }
                    }
            );
        }

        protected:
        // Mutex for the streams
        std::mutex mutex;

        // All subscriptions to this event stream.
        std::vector<EventSubscription<EventType>> subscriptions;
    };
}// namespace ECFMP::EventBus
