#pragma once
#include "ECFMP/event/Event.h"
#include "ECFMP/flightinformationregion/FlightInformationRegion.h"
#include "ECFMP/flowmeasure/FlowMeasure.h"
#include "api/InternalApiElementCollection.h"
#include "api/InternalStringIdentifierApiElementCollection.h"

namespace ECFMP::Api {
    typedef ECFMP::Api::InternalStringIdentifierApiElementCollection<
            ECFMP::FlightInformationRegion::FlightInformationRegion>
            InternalFlightInformationRegionCollection;

    typedef ECFMP::Api::InternalApiElementCollection<ECFMP::Event::Event> InternalEventCollection;

    typedef ECFMP::Api::InternalStringIdentifierApiElementCollection<ECFMP::FlowMeasure::FlowMeasure>
            InternalFlowMeasureCollection;
}// namespace ECFMP::Api
