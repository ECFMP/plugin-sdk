#pragma once

namespace ECFMP::FlowMeasure {
    class FlowMeasure;
}// namespace ECFMP::FlowMeasure

namespace ECFMP::Plugin {
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
