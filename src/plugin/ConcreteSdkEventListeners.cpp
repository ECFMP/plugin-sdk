#include "ConcreteSdkEventListeners.h"
#include "flow-sdk/EventListeners.h"

namespace FlowSdk::Plugin {
    ConcreteSdkEventListeners::ConcreteSdkEventListeners(
            std::unique_ptr<FlowMeasureActivatedEventListeners> notifiedListeners,
            std::unique_ptr<FlowMeasureNotifiedEventListeners> activatedListeners,
            std::unique_ptr<FlowMeasureExpiredEventListeners> expiredListeners,
            std::unique_ptr<FlowMeasureWithdrawnEventListeners> withdrawnListeners,
            std::unique_ptr<FlowMeasureReissuedEventListeners> reissuedListeners
    )
        : notifiedListeners(std::move(notifiedListeners)), activatedListeners(std::move(activatedListeners)),
          expiredListeners(std::move(expiredListeners)), withdrawnListeners(std::move(withdrawnListeners)),
          reissuedListeners(std::move(reissuedListeners))
    {
        assert(this->notifiedListeners && "Notified listeners not set in ConcreteSdk");
        assert(this->activatedListeners && "Activated listeners not set in ConcreteSdk");
        assert(this->withdrawnListeners && "Withdrawn listeners not set in ConcreteSdk");
        assert(this->expiredListeners && "Expired listeners not set in ConcreteSdk");
        assert(this->reissuedListeners && "Reissued listeners not set in ConcreteSdk");
    }

    auto ConcreteSdkEventListeners::FlowMeasureNotifiedListeners() const noexcept -> FlowMeasureNotifiedEventListeners&
    {
        return *notifiedListeners;
    }

    auto ConcreteSdkEventListeners::FlowMeasureActivatedListeners() const noexcept
            -> FlowMeasureActivatedEventListeners&
    {
        return *activatedListeners;
    }

    auto ConcreteSdkEventListeners::FlowMeasureExpiredListeners() const noexcept -> FlowMeasureExpiredEventListeners&
    {
        return *expiredListeners;
    }

    auto ConcreteSdkEventListeners::FlowMeasureWithdrawnListeners() const noexcept
            -> FlowMeasureWithdrawnEventListeners&
    {
        return *withdrawnListeners;
    }

    auto ConcreteSdkEventListeners::FlowMeasureReissuedListeners() const noexcept -> FlowMeasureReissuedEventListeners&
    {
        return *reissuedListeners;
    }
}// namespace FlowSdk::Plugin
