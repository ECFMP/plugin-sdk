#pragma once
#include "api/InternalElementCollectionTypes.h"

namespace ECFMP::Plugin {

    // API
    using ApiDataDownloadRequiredEvent = struct ApiDataDownloadRequiredEvent {
    };

    // FIRs
    using FlightInformationRegionsUpdatedEvent = struct FlightInformationRegionsUpdatedEvent {
        std::shared_ptr<Api::InternalFlightInformationRegionCollection> firs;
    };

    using InternalFlowMeasuresUpdatedEvent = struct InternalFlowMeasuresUpdatedEvent {
        std::shared_ptr<Api::InternalFlowMeasureCollection> flowMeasures;
    };

    // EuroScope
    using EuroscopeTimerTickEvent = struct EuroscopeTimerTickEvent {
    };
}// namespace ECFMP::Plugin
