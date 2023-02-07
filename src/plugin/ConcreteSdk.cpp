#include "ConcreteSdk.h"
#include "flow-sdk/EventListeners.h"

namespace FlowSdk::Plugin {
    ConcreteSdk::ConcreteSdk(
            std::unique_ptr<EventListeners<FlowMeasure::FlowMeasure>> notifiedListeners,
            std::unique_ptr<EventListeners<FlowMeasure::FlowMeasure>> activatedListeners,
            std::unique_ptr<EventListeners<FlowMeasure::FlowMeasure>> expiredListeners,
            std::unique_ptr<EventListeners<FlowMeasure::FlowMeasure>> withdrawnListeners,
            std::unique_ptr<EventListeners<FlowMeasure::FlowMeasure, FlowMeasure::FlowMeasure>> reissuedListeners)
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

    auto ConcreteSdk::FlowMeasureNotifiedListeners() const noexcept -> const EventListeners<FlowMeasure::FlowMeasure>&
    {
        return *notifiedListeners;
    }

    auto ConcreteSdk::FlowMeasureActivatedListeners() const noexcept -> const EventListeners<FlowMeasure::FlowMeasure>&
    {
        return *activatedListeners;
    }

    auto ConcreteSdk::FlowMeasureExpiredListeners() const noexcept -> const EventListeners<FlowMeasure::FlowMeasure>&
    {
        return *expiredListeners;
    }

    auto ConcreteSdk::FlowMeasureWithdrawnListeners() const noexcept -> const EventListeners<FlowMeasure::FlowMeasure>&
    {
        return *withdrawnListeners;
    }

    auto ConcreteSdk::FlowMeasureReissuedListeners() const noexcept
            -> const EventListeners<FlowMeasure::FlowMeasure, FlowMeasure::FlowMeasure>&
    {
        return *reissuedListeners;
    }
}// namespace FlowSdk::Plugin
