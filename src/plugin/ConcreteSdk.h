#pragma once
#include "flow-sdk/Sdk.h"

namespace FlowSdk::Api {
    class ApiDataScheduler;
}// namespace FlowSdk::Api

namespace FlowSdk::Plugin {

    class ConcreteSdk : public Sdk
    {
        public:
        ConcreteSdk(std::unique_ptr<Api::ApiDataScheduler> apiScheduler,
                    std::unique_ptr<SdkEventListeners> eventListeners);

        [[nodiscard]] auto Listeners() const noexcept -> SdkEventListeners& override;
        void Destroy() override;

        private:
        // Schedulers API downloads
        std::unique_ptr<Api::ApiDataScheduler> apiScheduler;

        // Contains event listeners
        std::unique_ptr<SdkEventListeners> eventListeners;
    };
}// namespace FlowSdk::Plugin
