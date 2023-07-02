#include "ConcreteSdk.h"
#include "api/ApiDataScheduler.h"
#include "flow-sdk/SdkEventListeners.h"

namespace FlowSdk::Plugin {
    ConcreteSdk::ConcreteSdk(
            std::unique_ptr<Api::ApiDataScheduler> apiScheduler, std::unique_ptr<SdkEventListeners> eventListeners
    )
        : apiScheduler(std::move(apiScheduler)), eventListeners(std::move(eventListeners))
    {
        assert(this->eventListeners && "Event listeners not set in ConcreteSdk");
        assert(this->apiScheduler && "Api scheduler not set in ConcreteSdk");
    }

    void ConcreteSdk::Destroy()
    {
        // Shutdown the API data downloader
        apiScheduler.reset();

        // Release the event listeners
        eventListeners.reset();
    }

    auto ConcreteSdk::Listeners() const noexcept -> SdkEventListeners&
    {
        return *eventListeners;
    }
}// namespace FlowSdk::Plugin
