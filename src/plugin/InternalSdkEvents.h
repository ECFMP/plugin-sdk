#pragma once
#include "api/InternalElementCollectionTypes.h"

namespace ECFMP::Plugin {

    // FIRs
    using FlightInformationRegionsUpdatedEvent = struct FlightInformationRegionsUpdatedEvent {
        std::shared_ptr<Api::InternalFlightInformationRegionCollection> firs;
    };
}// namespace ECFMP::Plugin
