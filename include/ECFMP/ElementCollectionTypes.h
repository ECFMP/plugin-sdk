#pragma once
#include "ApiElementCollection.h"
#include "FlightInformationRegion.h"

namespace ECFMP::Api {
    typedef ECFMP::Api::StringIdentifiedApiElementCollection<ECFMP::FlightInformationRegion::FlightInformationRegion>
            FlightInformationRegionCollection;
}// namespace ECFMP::Api
