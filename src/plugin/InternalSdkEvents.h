#pragma once
#include "api/InternalElementCollectionTypes.h"

namespace ECFMP::Plugin {

    // FIRs
    using FlightInformationRegionsUpdatedEvent = struct FlightInformationRegionsUpdatedEvent {
        std::shared_ptr<Api::InternalFlightInformationRegionCollection> firs;
    };

    // EuroScope
    using EuroscopeTimerTickEvent = struct EuroscopeTimerTickEvent {
    };
}// namespace ECFMP::Plugin
