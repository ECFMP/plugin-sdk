#pragma once

namespace FlowSdk::FlowMeasure {
    class FlowMeasure;
}// namespace FlowSdk::FlowMeasure

namespace FlowSdk::Plugin {

    class SdkEventListeners;

    /**
     * Represents the public-facing facade of the SDK.
     */
    class Sdk
    {
        public:
        virtual ~Sdk() = default;

        [[nodiscard]] virtual auto Listeners() const noexcept -> SdkEventListeners& = 0;

        /**
         * Destroys this SDK instance. Once destroyed it cannot be reused.
         */
        virtual void Destroy() = 0;
    };
}// namespace FlowSdk::Plugin
