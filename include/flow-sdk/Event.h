#pragma once
#include "EventParticipant.h"
#include "FlightInformationRegion.h"

namespace FlowSdk::Event {
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
        [[nodiscard]] virtual auto Id() const noexcept -> int = 0;

        /**
         * Returns the name of the event.
         */
        [[nodiscard]] virtual auto Name() const noexcept -> const std::string& = 0;

        /**
         * The start time of the event as a unix timestamp.
         */
        [[nodiscard]] virtual auto Start() const noexcept -> const std::chrono::system_clock::time_point& = 0;

        /**
         * The end time of the event as a unix timestamp.
         */
        [[nodiscard]] virtual auto End() const noexcept -> const std::chrono::system_clock::time_point& = 0;

        /**
         * The flight information region that is hosting the event.
         */
        [[nodiscard]] virtual auto FlightInformationRegion() const noexcept
                -> const FlightInformationRegion::FlightInformationRegion& = 0;

        /**
         * The VATCAN events code associated with the event, if one exists. NULL otherwise.
         */
        [[nodiscard]] virtual auto VatcanCode() const noexcept -> const std::string& = 0;

        /**
         * Compares events.
         */
        [[nodiscard]] auto operator==(const class Event& event) const -> bool;
    };
}// namespace FlowSdk::Event
