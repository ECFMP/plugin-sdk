#include "ConcreteFlightInformationRegionCollection.h"
#include "flow-sdk/FlightInformationRegion.h"

namespace FlowSdk::FlightInformationRegion {

    auto ConcreteFlightInformationRegionCollection::Get(int id) const -> std::shared_ptr<class FlightInformationRegion>
    {
        return firsById.contains(id) ? firsById.at(id) : nullptr;
    }

    auto ConcreteFlightInformationRegionCollection::Get(const std::string& identifier) const
            -> std::shared_ptr<class FlightInformationRegion>
    {
        return firsByIdentifier.contains(identifier) ? firsByIdentifier.at(identifier) : nullptr;
    }

    void ConcreteFlightInformationRegionCollection::Add(const std::shared_ptr<class FlightInformationRegion>& fir)
    {
        firsById[fir->Id()] = fir;
        firsByIdentifier[fir->Identifier()] = fir;
    }

    auto ConcreteFlightInformationRegionCollection::Count() const -> size_t
    {
        return firsById.size();
    }
}// namespace FlowSdk::FlightInformationRegion
