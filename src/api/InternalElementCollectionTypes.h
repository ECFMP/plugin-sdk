#pragma once
#include "ECFMP/event/Event.h"
#include "ECFMP/flightinformationregion/FlightInformationRegion.h"
#include "ECFMP/flowmeasure/FlowMeasure.h"
#include "InternalApiElementCollection.h"
#include "InternalStringIdentifiedApiElementCollection.h"

namespace ECFMP::Api {
    typedef ECFMP::Api::InternalStringIdentifiedApiElementCollection<
            ECFMP::FlightInformationRegion::FlightInformationRegion>
            InternalFlightInformationRegionCollection;

    typedef ECFMP::Api::InternalApiElementCollection<ECFMP::Event::Event> InternalEventCollection;
    typedef ECFMP::Api::InternalStringIdentifiedApiElementCollection<ECFMP::FlowMeasure::FlowMeasure>
            InternalFlowMeasureCollection;
}// namespace ECFMP::Api
