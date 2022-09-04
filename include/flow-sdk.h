#pragma once

namespace FlowSdk {
    namespace FlightInformationRegion {
        class FlightInformationRegion
        {
            public:
            virtual ~FlightInformationRegion() = default;
            [[nodiscard]] virtual auto Id() const -> int = 0;
            [[nodiscard]] virtual auto Identifier() const -> const char* = 0;
            [[nodiscard]] virtual auto Name() const -> const char* = 0;
        };
    }// namespace FlightInformationRegion
}// namespace FlowSdk
