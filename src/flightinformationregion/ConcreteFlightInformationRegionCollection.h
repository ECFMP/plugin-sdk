#pragma once
#include "flightinformationregion/InternalFlightInformationRegionCollection.h"

namespace FlowSdk::FlightInformationRegion {

    /**
     * Implements the flight information region collection
     */
    class ConcreteFlightInformationRegionCollection : public InternalFlightInformationRegionCollection
    {
        public:
        void Add(const std::shared_ptr<class FlightInformationRegion>& fir) override;
        [[nodiscard]] auto Count() const -> size_t;
        [[nodiscard]] auto Get(int id) const -> std::shared_ptr<class FlightInformationRegion> override;
        [[nodiscard]] auto Get(const std::string& identifier) const
                -> std::shared_ptr<class FlightInformationRegion> override;

        private:
        // The FIRs, by iD
        std::map<int, std::shared_ptr<class FlightInformationRegion>> firsById;

        // The FIRs, by identifier (e.g. EGTT)
        std::map<std::string, std::shared_ptr<class FlightInformationRegion>> firsByIdentifier;
    };

}// namespace FlowSdk::FlightInformationRegion
