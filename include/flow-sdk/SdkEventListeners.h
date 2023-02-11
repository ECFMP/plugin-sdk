#pragma once

namespace FlowSdk::FlowMeasure {
    class FlowMeasure;
}// namespace FlowSdk::FlowMeasure

namespace FlowSdk::Plugin {

    template<class... Types>
    class EventListeners;

    /**
     * The listeners for SDK events.
     */
    class SdkEventListeners
    {
        public:
        virtual ~SdkEventListeners() = default;

        /**
         * Returns the event listener collection for flow measure activated events.
         */
        [[nodiscard]] virtual auto FlowMeasureNotifiedListeners() const noexcept
                -> EventListeners<FlowMeasure::FlowMeasure>& = 0;

        /**
         * Returns the event listener collection for flow measure activated events.
         */
        [[nodiscard]] virtual auto FlowMeasureActivatedListeners() const noexcept
                -> EventListeners<FlowMeasure::FlowMeasure>& = 0;

        /**
         * Returns the event listener collection for flow measure expired events.
         */
        [[nodiscard]] virtual auto FlowMeasureExpiredListeners() const noexcept
                -> EventListeners<FlowMeasure::FlowMeasure>& = 0;

        /**
         * Returns the event listener collection for flow measure withdrawn events.
         */
        [[nodiscard]] virtual auto FlowMeasureWithdrawnListeners() const noexcept
                -> EventListeners<FlowMeasure::FlowMeasure>& = 0;

        /**
         * Returns the event listener collection for flow measure reissued events.
         */
        [[nodiscard]] virtual auto FlowMeasureReissuedListeners() const noexcept
                -> EventListeners<FlowMeasure::FlowMeasure, FlowMeasure::FlowMeasure>& = 0;
    };
}// namespace FlowSdk::Plugin
