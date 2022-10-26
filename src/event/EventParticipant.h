#pragma once

namespace FlowSdk::Event {
    class EventParticipant
    {
        public:
        [[nodiscard]] virtual auto Cid() const -> int = 0;
        [[nodiscard]] virtual auto OriginAirfield() const -> const char* = 0;
        [[nodiscard]] virtual auto DestinationAirfield() const -> const char* = 0;
    };
}// namespace FlowSdk::Event
