#pragma once
#include "ApiElementCollection.h"
#include "FlightInformationRegion.h"

namespace FlowSdk::Api {
    typedef FlowSdk::Api::StringIdentifiedApiElementCollection<
            FlowSdk::FlightInformationRegion::FlightInformationRegion>
            FlightInformationRegionCollection;
}// namespace FlowSdk::Api
