#pragma once
#include "ECFMP/eventbus/EventBus.h"

namespace ECFMP::EventBus {
    class InternalEventBus : public EventBus
    {
        public:
        template<typename EventType>
        void OnEvent(const EventType& event)
        {
            auto lock = std::lock_guard(pendingMutex);
            pendingEvents.push_back([event, this]() {
                GetStream<EventType>().OnEvent(event);
            });
        }

        /**
         * Processes all pending events.
         *
         * This method should be called from the EuroScope thread.
         *
         * This is necessary because we're expecting our users to use EuroScope classes such as FlightPlan,
         * which can only be used on the ES thread.
         */
        void ProcessPendingEvents()
        {
            auto lock = std::lock_guard(pendingMutex);
            for (const auto& event: pendingEvents) {
                event();
            }
            pendingEvents.clear();
        }

        private:
        // Protects the pending events
        std::mutex pendingMutex;

        // Events that are waiting to be processed by the EuroScope thread.
        std::vector<std::function<void()>> pendingEvents;
    };
}// namespace ECFMP::EventBus
