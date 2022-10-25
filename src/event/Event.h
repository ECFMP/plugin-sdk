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
        [[nodiscard]] virtual auto Name() const -> const std::string& = 0;
        [[nodiscard]] virtual auto Start() const -> const std::chrono::system_clock::time_point& = 0;
        [[nodiscard]] virtual auto End() const -> const std::chrono::system_clock::time_point& = 0;
        [[nodiscard]] virtual auto FlightInformationRegion() const
                -> const FlightInformationRegion::FlightInformationRegion& = 0;
    };
}// namespace FlowSdk::Event
