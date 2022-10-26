#pragma once

namespace FlowSdk::FlightInformationRegion {
    class FlightInformationRegion;
}// namespace FlowSdk::FlightInformationRegion

namespace FlowSdk::Event {
    class Event
    {
        public:
        virtual ~Event() = default;
        [[nodiscard]] virtual auto Id() const -> int = 0;
        [[nodiscard]] virtual auto Name() const -> const char* = 0;
        [[nodiscard]] virtual auto Start() const -> unsigned int = 0;
        [[nodiscard]] virtual auto End() const -> unsigned int = 0;
        [[nodiscard]] virtual auto FlightInformationRegion() const
                -> const FlightInformationRegion::FlightInformationRegion& = 0;
        [[nodiscard]] virtual auto VatcanCode() const -> const char* = 0;
    };
}// namespace FlowSdk::Event
