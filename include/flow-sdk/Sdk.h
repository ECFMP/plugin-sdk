#pragma once

namespace FlowSdk {
    namespace EventBus {
        class EventBus;
    }// namespace EventBus
    namespace FlowMeasure {
        class FlowMeasure;
    }// namespace FlowMeasure
}// namespace FlowSdk

namespace FlowSdk::Plugin {

    /**
     * Represents the public-facing facade of the SDK.
     */
    class Sdk
    {
        public:
        virtual ~Sdk() = default;

        /**
         * The event bus for the SDK, which can be used to subscribe to events.
         */
        [[nodiscard]] virtual auto EventBus() const noexcept -> EventBus::EventBus& = 0;

        /**
         * This method should be called once every second by the plugin using this SDK (as part of EuroScopes timer
         * tick method).
         */
        virtual void OnEuroscopeTimerTick() = 0;

        /**
         * Destroys this SDK instance. Once destroyed it cannot be reused.
         */
        virtual void Destroy() = 0;
    };
}// namespace FlowSdk::Plugin
