#pragma once
#include "FlightInformationRegion.h"

namespace FlowSdk::FlightInformationRegion {
    class ConcreteFlightInformationRegion : public FlightInformationRegion
    {
        public:
        ConcreteFlightInformationRegion(int id, const char* identifier, const char* name);

        [[nodiscard]] auto Id() const -> int;
        [[nodiscard]] auto Identifier() const -> const char*;
        [[nodiscard]] auto Name() const -> const char*;

        private:
        // ID of the FIR
        const int id;

        private:
        // The FIR identifier, e.g. EGTT
        const char* identifier;

        // The name, e.g. London
        const char* name;
    };
}// namespace FlowSdk::FlightInformationRegion
