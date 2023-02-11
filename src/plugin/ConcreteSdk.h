#pragma once
#include "flow-sdk/Sdk.h"

namespace FlowSdk::Plugin {

    class ConcreteSdk : public Sdk
    {
        public:
        ConcreteSdk(std::unique_ptr<SdkEventListeners> eventListeners);

        [[nodiscard]] auto Listeners() const noexcept -> SdkEventListeners& override;
        void Destroy() override;

        private:
        // Contains event listeners
        std::unique_ptr<SdkEventListeners> eventListeners;
    };
}// namespace FlowSdk::Plugin
