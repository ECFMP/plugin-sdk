#pragma once
#include "flow-sdk/SdkEventListenerTypes.h"
#include "flow-sdk/SdkEventListeners.h"

namespace FlowSdk::Plugin {
    class ConcreteSdkEventListeners : public SdkEventListeners
    {
        public:
        ConcreteSdkEventListeners(
                std::unique_ptr<FlowMeasureNotifiedEventListeners> notifiedListeners,
                std::unique_ptr<FlowMeasureActivatedEventListeners> activatedListeners,
                std::unique_ptr<FlowMeasureExpiredEventListeners> expiredListeners,
                std::unique_ptr<FlowMeasureWithdrawnEventListeners> withdrawnListeners,
                std::unique_ptr<FlowMeasureReissuedEventListeners> reissuedListeners
        );
        [[nodiscard]] auto FlowMeasureNotifiedListeners() const noexcept -> FlowMeasureNotifiedEventListeners& override;
        [[nodiscard]] auto FlowMeasureActivatedListeners() const noexcept
                -> FlowMeasureActivatedEventListeners& override;
        [[nodiscard]] auto FlowMeasureExpiredListeners() const noexcept -> FlowMeasureExpiredEventListeners& override;
        [[nodiscard]] auto FlowMeasureWithdrawnListeners() const noexcept
                -> FlowMeasureWithdrawnEventListeners& override;
        [[nodiscard]] auto FlowMeasureReissuedListeners() const noexcept -> FlowMeasureReissuedEventListeners& override;

        private:
        // The "notified" listeners
        std::unique_ptr<FlowMeasureNotifiedEventListeners> notifiedListeners;

        // The "activated" listeners
        std::unique_ptr<FlowMeasureActivatedEventListeners> activatedListeners;

        // The "expired" listeners
        std::unique_ptr<FlowMeasureExpiredEventListeners> expiredListeners;

        // The "withdrawn" listeners
        std::unique_ptr<FlowMeasureWithdrawnEventListeners> withdrawnListeners;

        // The "reissued" listeners
        std::unique_ptr<FlowMeasureReissuedEventListeners> reissuedListeners;
    };
}// namespace FlowSdk::Plugin
