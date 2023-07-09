#pragma once
#include "InternalApiElementCollection.h"
#include "InternalStringIdentifiedApiElementCollection.h"
#include "ECFMP/Event.h"
#include "ECFMP/FlightInformationRegion.h"
#include "ECFMP/FlowMeasure.h"

namespace ECFMP::Api {
    typedef ECFMP::Api::InternalStringIdentifiedApiElementCollection<
            ECFMP::FlightInformationRegion::FlightInformationRegion>
            InternalFlightInformationRegionCollection;

    typedef ECFMP::Api::InternalApiElementCollection<ECFMP::Event::Event> InternalEventCollection;
    typedef ECFMP::Api::InternalStringIdentifiedApiElementCollection<ECFMP::FlowMeasure::FlowMeasure>
            InternalFlowMeasureCollection;
}// namespace ECFMP::Api
