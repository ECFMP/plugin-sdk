#include "InternalSdk.h"
#include "InternalSdkEvents.h"
#include "eventbus/InternalEventBus.h"

namespace ECFMP::Plugin {
    InternalSdk::InternalSdk(
            std::shared_ptr<EventBus::InternalEventBus> eventBus,
            std::shared_ptr<std::vector<std::shared_ptr<FlowMeasure::CustomFlowMeasureFilter>>> customFlowMeasureFilters
    )
        : eventBus(std::move(eventBus)),
          flightInformationRegions(std::make_shared<Api::InternalFlightInformationRegionCollection>()),
          events(std::make_shared<Api::InternalEventCollection>()),
          flowMeasures(std::make_shared<Api::InternalFlowMeasureCollection>()),
          customFlowMeasureFilters(std::move(customFlowMeasureFilters))
    {
        assert(this->eventBus && "Event bus not set in ConcreteSdk");
        assert(this->customFlowMeasureFilters && "Custom flow measure filters not set in ConcreteSdk");
    }

    void InternalSdk::Destroy()
    {
        eventBus.reset();
    }

    auto InternalSdk::EventBus() const noexcept -> EventBus::EventBus&
    {
        return *eventBus;
    }

    void InternalSdk::OnEuroscopeTimerTick()
    {
        eventBus->OnEvent<EuroscopeTimerTickEvent>(EuroscopeTimerTickEvent{});
    }

    auto InternalSdk::FlightInformationRegions() const noexcept
            -> std::shared_ptr<const Api::FlightInformationRegionCollection>
    {
        auto lock = std::lock_guard(mutex);
        return flightInformationRegions;
    }

    void InternalSdk::OnEvent(const FlightInformationRegionsUpdatedEvent& event)
    {
        auto lock = std::lock_guard(mutex);
        flightInformationRegions = event.firs;
    }

    auto InternalSdk::Events() const noexcept -> std::shared_ptr<const Api::EventCollection>
    {
        auto lock = std::lock_guard(mutex);
        return events;
    }

    void InternalSdk::OnEvent(const EventsUpdatedEvent& event)
    {
        auto lock = std::lock_guard(mutex);
        events = event.events;
    }

    auto InternalSdk::FlowMeasures() const noexcept -> std::shared_ptr<const Api::FlowMeasureCollection>
    {
        auto lock = std::lock_guard(mutex);
        return flowMeasures;
    }

    void InternalSdk::OnEvent(const FlowMeasuresUpdatedEvent& eventType)
    {
        auto lock = std::lock_guard(mutex);
        flowMeasures = eventType.flowMeasures;
    }

    auto InternalSdk::CustomFlowMeasureFilters() const noexcept
            -> const std::vector<std::shared_ptr<FlowMeasure::CustomFlowMeasureFilter>>&
    {
        return *customFlowMeasureFilters;
    }
}// namespace ECFMP::Plugin
