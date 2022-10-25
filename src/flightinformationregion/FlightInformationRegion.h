#pragma once

namespace FlowSdk::FlightInformationRegion {
    class FlightInformationRegion
    {
        public:
        virtual ~FlightInformationRegion() = default;
        [[nodiscard]] virtual auto Id() const -> int = 0;
        [[nodiscard]] virtual auto Identifier() const -> const std::string& = 0;
        [[nodiscard]] virtual auto Name() const -> const std::string& = 0;
    };
}// namespace FlowSdk::FlightInformationRegion
