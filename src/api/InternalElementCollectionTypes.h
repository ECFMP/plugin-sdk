#pragma once
#include "InternalApiElementCollection.h"
#include "InternalStringIdentifiedApiElementCollection.h"
#include "flow-sdk/Event.h"
#include "flow-sdk/FlightInformationRegion.h"

namespace FlowSdk::Api {
    typedef FlowSdk::Api::InternalStringIdentifiedApiElementCollection<
            FlowSdk::FlightInformationRegion::FlightInformationRegion>
            InternalFlightInformationRegionCollection;

    typedef FlowSdk::Api::InternalApiElementCollection<FlowSdk::Event::Event> InternalEventCollection;
}// namespace FlowSdk::Api
