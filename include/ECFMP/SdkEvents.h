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
        const FlowMeasure::FlowMeasure& measure;
    };

    using FlowMeasureActivatedEvent = struct FlowMeasureActivatedEvent {
        const FlowMeasure::FlowMeasure& measure;
    };

    using FlowMeasureExpiredEvent = struct FlowMeasureExpiredEvent {
        const FlowMeasure::FlowMeasure& measure;
    };

    using FlowMeasureWithdrawnEvent = struct FlowMeasureWithdrawnEvent {
        const FlowMeasure::FlowMeasure& measure;
    };

    using FlowMeasureReissuedEvent = struct FlowMeasureReissuedEvent {
        const FlowMeasure::FlowMeasure& original;
        const FlowMeasure::FlowMeasure& reissued;
    };
}// namespace ECFMP::Plugin
