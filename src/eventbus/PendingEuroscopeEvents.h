#pragma once
#include "ECFMP/eventbus/EventListener.h"
#include "EventStream.h"
#include "plugin/InternalSdkEvents.h"

namespace ECFMP::EventBus {

    /**
     * Stores pending ES events
     */
    class PendingEuroscopeEvents : public EventListener<Plugin::EuroscopeTimerTickEvent>
    {
        public:
        void AddEvent(const std::function<void()>& event);
        void OnEvent(const Plugin::EuroscopeTimerTickEvent& timerTickEvent) override;

        private:
        // Mutex for the pending events
        std::mutex pendingEventsMutex;

        // The pending events
        std::vector<std::function<void()>> pendingEvents;
    };
}// namespace ECFMP::EventBus
