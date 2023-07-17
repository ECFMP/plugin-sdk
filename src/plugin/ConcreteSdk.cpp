#include "ConcreteSdk.h"
#include "eventbus/InternalEventBus.h"

namespace ECFMP::Plugin {
    ConcreteSdk::ConcreteSdk(std::shared_ptr<void> apiScheduler, std::shared_ptr<EventBus::InternalEventBus> eventBus)
        : apiScheduler(std::move(apiScheduler)), eventBus(std::move(eventBus)),
          flightInformationRegions(std::make_shared<Api::InternalFlightInformationRegionCollection>())
    {
        assert(this->apiScheduler && "Api scheduler not set in ConcreteSdk");
        assert(this->eventBus && "Event bus not set in ConcreteSdk");
    }

    void ConcreteSdk::Destroy()
    {
        // Shutdown the API data downloader
        apiScheduler.reset();
    }

    auto ConcreteSdk::EventBus() const noexcept -> EventBus::EventBus&
    {
        return *eventBus;
    }

    void ConcreteSdk::OnEuroscopeTimerTick()
    {
        eventBus->ProcessPendingEvents();
    }

    auto ConcreteSdk::FlightInformationRegions() const noexcept
            -> std::shared_ptr<const Api::FlightInformationRegionCollection>
    {
        auto lock = std::lock_guard(mutex);
        return flightInformationRegions;
    }

    void ConcreteSdk::OnEvent(const FlightInformationRegionsUpdatedEvent& event)
    {
        auto lock = std::lock_guard(mutex);
        flightInformationRegions = event.firs;
    }
}// namespace ECFMP::Plugin
