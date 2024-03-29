#pragma once
#include "ECFMP/api/ApiElement.h"
#include "ECFMP/flightinformationregion/FlightInformationRegion.h"
#include "EventParticipant.h"

namespace ECFMP::Event {
    /**
     * Represents an event on the network.
     */
    class Event : public Api::ApiElement
    {
        public:
        virtual ~Event() = default;

        /**
         * The id of the event in the API.
         */
        [[nodiscard]] auto Id() const noexcept -> int override = 0;

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
         * Returns the participants of the event.
         */
        [[nodiscard]] virtual auto Participants() const -> const std::vector<std::shared_ptr<EventParticipant>>& = 0;

        /**
         * Compares events.
         */
        [[nodiscard]] auto operator==(const class Event& event) const -> bool;
    };
}// namespace ECFMP::Event
