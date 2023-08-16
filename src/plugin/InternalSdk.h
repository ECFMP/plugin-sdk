#pragma once
#include "ECFMP/Sdk.h"
#include "ECFMP/SdkEvents.h"
#include "ECFMP/api/ElementCollectionTypes.h"
#include "ECFMP/eventbus/EventListener.h"
#include "InternalSdkEvents.h"
#include "api/InternalElementCollectionTypes.h"

namespace ECFMP {
    namespace EventBus {
        class InternalEventBus;
    }// namespace EventBus
    namespace FlowMeasure {
        class CustomFlowMeasureFilter;
    }// namespace FlowMeasure
}// namespace ECFMP

namespace ECFMP::Plugin {

    class InternalSdk : public Sdk,
                        public EventBus::EventListener<Plugin::FlightInformationRegionsUpdatedEvent>,
                        public EventBus::EventListener<Plugin::EventsUpdatedEvent>,
                        public EventBus::EventListener<Plugin::FlowMeasuresUpdatedEvent>
    {
        public:
        InternalSdk(
                std::shared_ptr<EventBus::InternalEventBus> eventBus,
                std::shared_ptr<std::vector<std::shared_ptr<FlowMeasure::CustomFlowMeasureFilter>>>
                        customFlowMeasureFilters
        );
        ~InternalSdk() override = default;

        [[nodiscard]] auto CustomFlowMeasureFilters() const noexcept
                -> const std::vector<std::shared_ptr<FlowMeasure::CustomFlowMeasureFilter>>&;
        [[nodiscard]] auto FlightInformationRegions() const noexcept
                -> std::shared_ptr<const Api::FlightInformationRegionCollection> override;
        [[nodiscard]] auto Events() const noexcept -> std::shared_ptr<const Api::EventCollection> override;
        [[nodiscard]] auto FlowMeasures() const noexcept -> std::shared_ptr<const Api::FlowMeasureCollection> override;

        [[nodiscard]] auto EventBus() const noexcept -> EventBus::EventBus& override;
        void OnEuroscopeTimerTick() override;
        void OnEvent(const FlightInformationRegionsUpdatedEvent& event) override;
        void OnEvent(const EventsUpdatedEvent& eventType) override;
        void OnEvent(const FlowMeasuresUpdatedEvent& eventType) override;

        void Destroy() override;

        private:
        // The event bus for the SDK, which can be used to subscribe to events.
        std::shared_ptr<EventBus::InternalEventBus> eventBus;

        // The flight information regions that are currently loaded.
        std::shared_ptr<Api::FlightInformationRegionCollection> flightInformationRegions;

        // The events that are currently loaded.
        std::shared_ptr<Api::EventCollection> events;

        // The flow measures that are currently loaded.
        std::shared_ptr<Api::FlowMeasureCollection> flowMeasures;

        // The custom flow measure filters that are currently loaded.
        std::shared_ptr<std::vector<std::shared_ptr<FlowMeasure::CustomFlowMeasureFilter>>> customFlowMeasureFilters;

        // Locks the class for returning the flight information regions.
        mutable std::mutex mutex;
    };
}// namespace ECFMP::Plugin
