#include "PendingEuroscopeEvents.h"

namespace ECFMP::EventBus {

    void PendingEuroscopeEvents::AddEvent(const std::function<void()>& event)
    {
        std::lock_guard<std::mutex> lock(pendingEventsMutex);
        pendingEvents.push_back(event);
    }

    void PendingEuroscopeEvents::OnEvent(const Plugin::EuroscopeTimerTickEvent& timerTickEvent)
    {
        std::lock_guard<std::mutex> lock(pendingEventsMutex);
        for (const auto& event: pendingEvents) {
            event();
        }

        pendingEvents.clear();
    }
}// namespace ECFMP::EventBus
