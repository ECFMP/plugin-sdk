#pragma once

namespace FlowSdk::Event {
    /**
     * Represents a participant in an event
     */
    class EventParticipant
    {
        public:
        virtual ~EventParticipant() = default;

        /**
         * Returns the VATSIM CID of the event participant.
         */
        [[nodiscard]] virtual auto Cid() const noexcept -> int = 0;

        /**
         * Returns the origin airfield of the event participants flight, or NULL if not found.
         */
        [[nodiscard]] virtual auto OriginAirfield() const noexcept -> const char* = 0;

        /**
         * Returns the destination airfield of the event participants flight, or NULL if not found.
         */
        [[nodiscard]] virtual auto DestinationAirfield() const noexcept -> const char* = 0;
    };
}// namespace FlowSdk::Event
