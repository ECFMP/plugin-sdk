#pragma once
#include "ECFMP/api/ElementCollectionTypes.h"

namespace ECFMP::FlowMeasure {
    class FlowMeasure;
}// namespace ECFMP::FlowMeasure

namespace ECFMP::Plugin {

    // Events
    using EventsUpdatedEvent = struct EventsUpdatedEvent {
        std::shared_ptr<ECFMP::Api::EventCollection> events;
    };

    // Flow measures
    using FlowMeasuresUpdatedEvent = struct FlowMeasureUpdatedEvent {
        std::shared_ptr<ECFMP::Api::FlowMeasureCollection> flowMeasures;
    };

    using FlowMeasureNotifiedEvent = struct FlowMeasureNotifiedEvent {
        std::shared_ptr<const FlowMeasure::FlowMeasure> flowMeasure;
    };

    using FlowMeasureActivatedEvent = struct FlowMeasureActivatedEvent {
        std::shared_ptr<const FlowMeasure::FlowMeasure> flowMeasure;
    };

    using FlowMeasureExpiredEvent = struct FlowMeasureExpiredEvent {
        std::shared_ptr<const FlowMeasure::FlowMeasure> flowMeasure;
    };

    using FlowMeasureWithdrawnEvent = struct FlowMeasureWithdrawnEvent {
        std::shared_ptr<const FlowMeasure::FlowMeasure> flowMeasure;
    };

    using FlowMeasureReissuedEvent = struct FlowMeasureReissuedEvent {
        std::shared_ptr<const FlowMeasure::FlowMeasure> original;
        std::shared_ptr<const FlowMeasure::FlowMeasure> reissued;
    };
}// namespace ECFMP::Plugin
