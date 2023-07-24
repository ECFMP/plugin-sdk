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

    // EuroScope
    using EuroscopeTimerTickEvent = struct EuroscopeTimerTickEvent {
    };
}// namespace ECFMP::Plugin
