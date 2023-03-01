#pragma once
#include "InternalStringIdentifiedApiElementCollection.h"
#include "flow-sdk/FlightInformationRegion.h"

namespace FlowSdk::Api {
    typedef FlowSdk::Api::InternalStringIdentifiedApiElementCollection<
            FlowSdk::FlightInformationRegion::FlightInformationRegion>
            InternalFlightInformationRegionCollection;
}
