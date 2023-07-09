#pragma once
#include "ECFMP/api/ApiElementCollection.h"
#include "ECFMP/flightinformationregion/FlightInformationRegion.h"

namespace ECFMP::Api {
    typedef ECFMP::Api::StringIdentifiedApiElementCollection<ECFMP::FlightInformationRegion::FlightInformationRegion>
            FlightInformationRegionCollection;
}// namespace ECFMP::Api
