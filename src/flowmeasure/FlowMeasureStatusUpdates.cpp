#include "FlowMeasureStatusUpdates.h"
#include "ECFMP/SdkEvents.h"
#include "ECFMP/flowmeasure/CanonicalFlowMeasureInfo.h"
#include "ECFMP/log/Logger.h"
#include "eventbus/InternalEventBus.h"

namespace ECFMP::FlowMeasure {

    FlowMeasureStatusUpdates::FlowMeasureStatusUpdates(
            std::shared_ptr<EventBus::InternalEventBus> eventBus, std::shared_ptr<Log::Logger> logger
    )
        : eventBus(std::move(eventBus)), logger(std::move(logger))
    {
        assert(this->eventBus != nullptr && "The event bus cannot be null.");
        assert(this->logger != nullptr && "The logger cannot be null.");
    }

    void FlowMeasureStatusUpdates::OnEvent(const Plugin::InternalFlowMeasuresUpdatedEvent& event)
    {
        for (const auto& measurePair: event.flowMeasures->GetUnderlyingCollection()) {
            const auto measure = measurePair.second;

            auto canonicalInformation = measure->CanonicalInformation();

            // Never seen it before, so we can only broadcast a state.
            if (!canonicalFlowMeasures.contains(canonicalInformation.Identifier())) {
                canonicalFlowMeasures[canonicalInformation.Identifier()] = measure;
                BroadcastStatusUpdate(measure);
                return;
            }

            // Switch the measure we have stored out for the new one
            auto previousMeasure = canonicalFlowMeasures[canonicalInformation.Identifier()];
            canonicalFlowMeasures[canonicalInformation.Identifier()] = measure;

            // If the new measure is after the old one, we need to reissue the measure
            if (canonicalInformation.IsAfter(previousMeasure->CanonicalInformation())) {
                eventBus->OnEvent<Plugin::FlowMeasureReissuedEvent>({previousMeasure, measure});
            }

            // If the status has changed, we need to broadcast the new status
            if (previousMeasure->Status() != measure->Status()) {
                BroadcastStatusUpdate(measure);
            }
        }
    }

    void FlowMeasureStatusUpdates::BroadcastStatusUpdate(const std::shared_ptr<const FlowMeasure>& measure)
    {
        switch (measure->Status()) {
        case MeasureStatus::Notified:
            WriteLogMessage(*measure, "Notified");
            eventBus->OnEvent<Plugin::FlowMeasureNotifiedEvent>({measure});
            break;
        case MeasureStatus::Active:
            WriteLogMessage(*measure, "Activated");
            eventBus->OnEvent<Plugin::FlowMeasureActivatedEvent>({measure});
            break;
        case MeasureStatus::Withdrawn:
            WriteLogMessage(*measure, "Withdrawn");
            eventBus->OnEvent<Plugin::FlowMeasureWithdrawnEvent>({measure});
            break;
        case MeasureStatus::Expired:
            WriteLogMessage(*measure, "Expired");
            eventBus->OnEvent<Plugin::FlowMeasureExpiredEvent>({measure});
            break;
        }
    }

    void FlowMeasureStatusUpdates::WriteLogMessage(const FlowMeasure& measure, const std::string& status)
    {
        logger->Info("Flow measure " + measure.Identifier() + " now has a status of " + status + ".");
    }
}// namespace ECFMP::FlowMeasure
