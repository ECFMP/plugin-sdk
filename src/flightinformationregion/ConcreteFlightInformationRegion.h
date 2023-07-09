#pragma once
#include "ECFMP/FlightInformationRegion.h"

namespace ECFMP::FlightInformationRegion {
    class ConcreteFlightInformationRegion : public FlightInformationRegion
    {
        public:
        ConcreteFlightInformationRegion(int id, std::string identifier, std::string name);

        [[nodiscard]] auto Id() const noexcept -> int override;
        [[nodiscard]] auto Identifier() const noexcept -> const std::string& override;
        [[nodiscard]] auto Name() const noexcept -> const std::string& override;

        private:
        // ID of the FIR
        const int id;

        private:
        // The FIR identifier, e.g. EGTT
        const std::string identifier;

        // The name, e.g. London
        const std::string name;
    };
}// namespace ECFMP::FlightInformationRegion
