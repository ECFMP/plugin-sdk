#pragma once

namespace FlowSdk::FlowMeasure {
    class FlowMeasure;
}// namespace FlowSdk::FlowMeasure

namespace FlowSdk::Plugin {
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
}// namespace FlowSdk::Plugin
