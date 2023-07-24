#include "ConcreteSdk.h"
#include "InternalSdkEvents.h"
#include "eventbus/InternalEventBus.h"

namespace ECFMP::Plugin {
    ConcreteSdk::ConcreteSdk(std::shared_ptr<EventBus::InternalEventBus> eventBus)
        : eventBus(std::move(eventBus)),
          flightInformationRegions(std::make_shared<Api::InternalFlightInformationRegionCollection>()),
          events(std::make_shared<Api::InternalEventCollection>()),
          flowMeasures(std::make_shared<Api::InternalFlowMeasureCollection>())
    {
        assert(this->eventBus && "Event bus not set in ConcreteSdk");
    }

    void ConcreteSdk::Destroy()
    {
        eventBus.reset();
    }

    auto ConcreteSdk::EventBus() const noexcept -> EventBus::EventBus&
    {
        return *eventBus;
    }

    void ConcreteSdk::OnEuroscopeTimerTick()
    {
        eventBus->OnEvent<EuroscopeTimerTickEvent>(EuroscopeTimerTickEvent{});
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

    auto ConcreteSdk::Events() const noexcept -> std::shared_ptr<const Api::EventCollection>
    {
        auto lock = std::lock_guard(mutex);
        return events;
    }

    void ConcreteSdk::OnEvent(const EventsUpdatedEvent& event)
    {
        auto lock = std::lock_guard(mutex);
        events = event.events;
    }

    auto ConcreteSdk::FlowMeasures() const noexcept -> std::shared_ptr<const Api::FlowMeasureCollection>
    {
        auto lock = std::lock_guard(mutex);
        return flowMeasures;
    }

    void ConcreteSdk::OnEvent(const FlowMeasuresUpdatedEvent& eventType)
    {
        auto lock = std::lock_guard(mutex);
        flowMeasures = eventType.flowMeasures;
    }
}// namespace ECFMP::Plugin
