#pragma once
#include "ECFMP/api/ApiElementCollection.h"
#include "ECFMP/api/StringIdentifierApiElementCollection.h"
#include "ECFMP/event/Event.h"
#include "ECFMP/flightinformationregion/FlightInformationRegion.h"
#include "ECFMP/flowmeasure/FlowMeasure.h"

namespace ECFMP::Api {

    typedef ECFMP::Api::StringIdentifierApiElementCollection<ECFMP::FlightInformationRegion::FlightInformationRegion>
            FlightInformationRegionCollection;

    typedef ECFMP::Api::ApiElementCollection<ECFMP::Event::Event> EventCollection;

    typedef ECFMP::Api::StringIdentifierApiElementCollection<ECFMP::FlowMeasure::FlowMeasure> FlowMeasureCollection;
}// namespace ECFMP::Api
