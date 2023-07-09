#pragma once
#include "InternalApiElementCollection.h"
#include "InternalStringIdentifiedApiElementCollection.h"
#include "flow-sdk/Event.h"
#include "flow-sdk/FlightInformationRegion.h"
#include "flow-sdk/FlowMeasure.h"

namespace ECFMP::Api {
    typedef ECFMP::Api::InternalStringIdentifiedApiElementCollection<
            ECFMP::FlightInformationRegion::FlightInformationRegion>
            InternalFlightInformationRegionCollection;

    typedef ECFMP::Api::InternalApiElementCollection<ECFMP::Event::Event> InternalEventCollection;
    typedef ECFMP::Api::InternalStringIdentifiedApiElementCollection<ECFMP::FlowMeasure::FlowMeasure>
            InternalFlowMeasureCollection;
}// namespace ECFMP::Api
