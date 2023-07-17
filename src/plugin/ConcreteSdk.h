#pragma once
#include "ECFMP/Sdk.h"
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

    class ConcreteSdk : public Sdk
    {
        public:
        ConcreteSdk(std::shared_ptr<void> apiScheduler, std::shared_ptr<EventBus::InternalEventBus> eventBus);
        ~ConcreteSdk() override = default;

        [[nodiscard]] auto FlightInformationRegions() const noexcept
                -> const Api::FlightInformationRegionCollection& override;

        [[nodiscard]] auto EventBus() const noexcept -> EventBus::EventBus& override;
        void OnEuroscopeTimerTick() override;

        void Destroy() override;

        private:
        // Schedules API downloads - this kinda keeps itself to itself, so we dont need to enforce types here
        // which makes things easier for testing
        std::shared_ptr<void> apiScheduler;

        // The event bus for the SDK, which can be used to subscribe to events.
        std::shared_ptr<EventBus::InternalEventBus> eventBus;

        // The flight information regions that are currently loaded.
        std::shared_ptr<Api::InternalFlightInformationRegionCollection> flightInformationRegions;
    };
}// namespace ECFMP::Plugin
