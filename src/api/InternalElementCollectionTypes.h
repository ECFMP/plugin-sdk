#pragma once
#include "InternalApiElementCollection.h"
#include "InternalStringIdentifiedApiElementCollection.h"
#include "flow-sdk/Event.h"
#include "flow-sdk/FlightInformationRegion.h"
#include "flow-sdk/FlowMeasure.h"

namespace FlowSdk::Api {
    typedef FlowSdk::Api::InternalStringIdentifiedApiElementCollection<
            FlowSdk::FlightInformationRegion::FlightInformationRegion>
            InternalFlightInformationRegionCollection;

    typedef FlowSdk::Api::InternalApiElementCollection<FlowSdk::Event::Event> InternalEventCollection;
    typedef FlowSdk::Api::InternalStringIdentifiedApiElementCollection<FlowSdk::FlowMeasure::FlowMeasure>
            InternalFlowMeasureCollection;
}// namespace FlowSdk::Api
