#pragma once
#include "FlightInformationRegion.h"

namespace FlowSdk::FlightInformationRegion {
    class ConcreteFlightInformationRegion : public FlightInformationRegion
    {
        public:
        ConcreteFlightInformationRegion(int id, std::string identifier, std::string name);

        [[nodiscard]] auto Id() const -> int;
        [[nodiscard]] auto Identifier() const -> const std::string&;
        [[nodiscard]] auto Name() const -> const std::string&;

        private:
        // ID of the FIR
        const int id;

        private:
        // The FIR identifier, e.g. EGTT
        const std::string identifier;

        // The name, e.g. London
        const std::string name;
    };
}// namespace FlowSdk::FlightInformationRegion
