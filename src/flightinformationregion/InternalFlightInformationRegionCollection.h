#pragma once
#include "FlightInformationRegionCollection.h"

namespace FlowSdk::FlightInformationRegion {
    /**
     * An internal interface for the FIR collection to add the Add method. Means that we don't have to expose
     * this to external users in the future if we don't want to.
     */
    class InternalFlightInformationRegionCollection : public FlightInformationRegionCollection
    {
        public:
        virtual void Add(const std::shared_ptr<class FlightInformationRegion>& fir) = 0;
    };
}// namespace FlowSdk::FlightInformationRegion
