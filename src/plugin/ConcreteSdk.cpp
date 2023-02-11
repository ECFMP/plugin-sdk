#include "ConcreteSdk.h"
#include "flow-sdk/SdkEventListeners.h"

namespace FlowSdk::Plugin {
    ConcreteSdk::ConcreteSdk(std::unique_ptr<SdkEventListeners> eventListeners)
        : eventListeners(std::move(eventListeners))
    {
        assert(this->eventListeners && "Event listeners not set in ConcreteSdk");
    }

    void ConcreteSdk::Destroy()
    {
        // No-op
    }

    auto ConcreteSdk::Listeners() const noexcept -> SdkEventListeners&
    {
        return *eventListeners;
    }
}// namespace FlowSdk::Plugin
