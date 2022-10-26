#pragma once

namespace FlowSdk {
    namespace FlightInformationRegion {

        /**
         * A flight information region.
         */
        class FlightInformationRegion
        {
            public:
            virtual ~FlightInformationRegion() = default;

            /**
             * The id on the API.
             */
            [[nodiscard]] virtual auto Id() const -> int = 0;

            /**
             * The identifier of the FIR, for example "EGTT".
             */
            [[nodiscard]] virtual auto Identifier() const -> const char* = 0;

            /**
             * The name of the FIR, for example "London"
             */
            [[nodiscard]] virtual auto Name() const -> const char* = 0;
        };
    }// namespace FlightInformationRegion

    namespace Event {
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
            [[nodiscard]] virtual auto Cid() const -> int = 0;

            /**
             * Returns the origin airfield of the event participants flight, or NULL if not found.
             */
            [[nodiscard]] virtual auto OriginAirfield() const -> const char* = 0;

            /**
             * Returns the destination airfield of the event participants flight, or NULL if not found.
             */
            [[nodiscard]] virtual auto DestinationAirfield() const -> const char* = 0;
        };

        /**
         * Represents an event on the network.
         */
        class Event
        {
            public:
            virtual ~Event() = default;

            /**
             * The id of the event in the API.
             */
            [[nodiscard]] virtual auto Id() const -> int = 0;

            /**
             * Returns the name of the event.
             */
            [[nodiscard]] virtual auto Name() const -> const char* = 0;

            /**
             * The start time of the event as a unix timestamp.
             */
            [[nodiscard]] virtual auto Start() const -> unsigned int = 0;

            /**
             * The end time of the event as a unix timestamp.
             */
            [[nodiscard]] virtual auto End() const -> unsigned int = 0;

            /**
             * The flight information region that is hosting the event.
             */
            [[nodiscard]] virtual auto FlightInformationRegion() const
                    -> const FlightInformationRegion::FlightInformationRegion& = 0;

            /**
             * The VATCAN events code associated with the event, if one exists. NULL otherwise.
             */
            [[nodiscard]] virtual auto VatcanCode() const -> const char* = 0;
        };
    }// namespace Event
}// namespace FlowSdk
