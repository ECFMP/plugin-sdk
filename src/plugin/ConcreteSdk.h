#pragma once
#include "ECFMP/Sdk.h"
#include "ECFMP/SdkEvents.h"
#include "ECFMP/api/ElementCollectionTypes.h"
#include "ECFMP/eventbus/EventListener.h"
#include "InternalSdkEvents.h"
#include "api/InternalElementCollectionTypes.h"

namespace ECFMP {
    namespace Api {
        class ApiDataScheduler;
    }
    namespace EventBus {
        class InternalEventBus;
    }
}// namespace ECFMP

namespace ECFMP::Plugin {

    class ConcreteSdk : public Sdk,
                        public EventBus::EventListener<Plugin::FlightInformationRegionsUpdatedEvent>,
                        public EventBus::EventListener<Plugin::EventsUpdatedEvent>
    {
        public:
        ConcreteSdk(std::shared_ptr<void> apiScheduler, std::shared_ptr<EventBus::InternalEventBus> eventBus);
        ~ConcreteSdk() override = default;

        [[nodiscard]] auto FlightInformationRegions() const noexcept
                -> std::shared_ptr<const Api::FlightInformationRegionCollection> override;
        [[nodiscard]] auto Events() const noexcept -> std::shared_ptr<const Api::EventCollection> override;

        [[nodiscard]] auto EventBus() const noexcept -> EventBus::EventBus& override;
        void OnEuroscopeTimerTick() override;
        void OnEvent(const FlightInformationRegionsUpdatedEvent& event) override;
        void OnEvent(const EventsUpdatedEvent& eventType) override;

        void Destroy() override;

        private:
        // Schedules API downloads - this kinda keeps itself to itself, so we dont need to enforce types here
        // which makes things easier for testing
        std::shared_ptr<void> apiScheduler;

        // The event bus for the SDK, which can be used to subscribe to events.
        std::shared_ptr<EventBus::InternalEventBus> eventBus;

        // The flight information regions that are currently loaded.
        std::shared_ptr<Api::FlightInformationRegionCollection> flightInformationRegions;

        // The events that are currently loaded.
        std::shared_ptr<Api::EventCollection> events;

        // Locks the class for returning the flight information regions.
        mutable std::mutex mutex;
    };
}// namespace ECFMP::Plugin
