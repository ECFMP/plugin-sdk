#pragma once
#include "ECFMP/eventbus/EventListener.h"
#include "plugin/InternalSdkEvents.h"

namespace ECFMP {
    namespace EventBus {
        class InternalEventBus;
    }// namespace EventBus
    namespace Log {
        class Logger;
    }// namespace Log
}// namespace ECFMP

namespace ECFMP::FlowMeasure {

    class FlowMeasure;

    /**
     * Handles status updates for flow measures.
     */
    class FlowMeasureStatusUpdates : public EventBus::EventListener<Plugin::InternalFlowMeasuresUpdatedEvent>
    {
        public:
        FlowMeasureStatusUpdates(
                std::shared_ptr<EventBus::InternalEventBus> eventBus, std::shared_ptr<Log::Logger> logger
        );
        void OnEvent(const Plugin::InternalFlowMeasuresUpdatedEvent& event) override;

        private:
        void BroadcastStatusUpdate(const std::shared_ptr<const FlowMeasure>& measure);

        // The event bus
        std::shared_ptr<EventBus::InternalEventBus> eventBus;

        // The logger
        std::shared_ptr<Log::Logger> logger;

        // The canonical flow measures
        std::unordered_map<std::string, std::shared_ptr<const FlowMeasure>> canonicalFlowMeasures;
    };

}// namespace ECFMP::FlowMeasure
