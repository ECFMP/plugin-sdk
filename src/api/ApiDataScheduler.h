#pragma once
#include "ECFMP/eventbus/EventListener.h"
#include "eventbus/InternalEventBus.h"
#include "plugin/InternalSdkEvents.h"

namespace ECFMP::Api {

    class ApiDataDownloader;

    /**
     * Responsible for downloading API data on a schedule.
     */
    class ApiDataScheduler : public EventBus::EventListener<Plugin::EuroscopeTimerTickEvent>
    {
        public:
        explicit ApiDataScheduler(std::shared_ptr<EventBus::InternalEventBus> eventBus);
        ~ApiDataScheduler() override;
        void OnEvent(const Plugin::EuroscopeTimerTickEvent& eventType) override;

        private:
        struct Impl;
        std::unique_ptr<Impl> impl;
    };

}// namespace ECFMP::Api
