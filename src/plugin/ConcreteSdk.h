#pragma once
#include "flow-sdk/Sdk.h"

namespace FlowSdk {
    namespace Api {
        class ApiDataScheduler;
    }
    namespace EventBus {
        class InternalEventBus;
    }
}// namespace FlowSdk

namespace FlowSdk::Plugin {

    class ConcreteSdk : public Sdk
    {
        public:
        ConcreteSdk(std::shared_ptr<void> apiScheduler, std::shared_ptr<EventBus::InternalEventBus> eventBus);
        ~ConcreteSdk() override = default;

        [[nodiscard]] auto EventBus() const noexcept -> EventBus::EventBus& override;
        void OnEuroscopeTimerTick() override;

        void Destroy() override;

        private:
        // Schedules API downloads - this kinda keeps itself to itself, so we dont need to enforce types here
        // which makes things easier for testing
        std::shared_ptr<void> apiScheduler;

        // The event bus for the SDK, which can be used to subscribe to events.
        std::shared_ptr<EventBus::InternalEventBus> eventBus;
    };
}// namespace FlowSdk::Plugin
