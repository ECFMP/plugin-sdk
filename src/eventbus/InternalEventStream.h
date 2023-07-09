#pragma once
#include "ECFMP/EventStream.h"
#include "ECFMP/NewEventFilter.h"
#include "ECFMP/NewEventListener.h"

namespace ECFMP::EventBus {

    template<typename EventType>
    class InternalEventStream : public EventStream<EventType>
    {
        public:
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

                                // Call the listener.
                                subscription.listener->OnEvent(event);

                                // For any called listeners that are only valid for one event, remove them.
                                return subscription.once;
                            }
                    ),
                    this->subscriptions.end()
            );
        };
    };
}// namespace ECFMP::EventBus
