#pragma once
#include "ECFMP/event/Event.h"

namespace ECFMP::FlightInformationRegion {
    class FlightInformationRegion;
}// namespace ECFMP::FlightInformationRegion

namespace ECFMP::Event {
    class ConcreteEvent : public Event::Event
    {
        public:
        ConcreteEvent(
                int id, std::string name, std::chrono::system_clock::time_point start,
                std::chrono::system_clock::time_point end,
                std::shared_ptr<const FlightInformationRegion::FlightInformationRegion> fir, std::string vatcanCode,
                std::vector<std::shared_ptr<EventParticipant>> participants
        );
        [[nodiscard]] auto Id() const noexcept -> int override;
        [[nodiscard]] auto Name() const noexcept -> const std::string& override;
        [[nodiscard]] auto Start() const noexcept -> const std::chrono::system_clock::time_point& override;
        [[nodiscard]] auto End() const noexcept -> const std::chrono::system_clock::time_point& override;
        [[nodiscard]] auto FlightInformationRegion() const noexcept
                -> const FlightInformationRegion::FlightInformationRegion& override;
        [[nodiscard]] auto VatcanCode() const noexcept -> const std::string& override;
        [[nodiscard]] auto Participants() const -> const std::vector<std::shared_ptr<EventParticipant>>& override;

        private:
        // Api event id
        const int id;

        // Event name
        std::string name;

        // The start time
        const std::chrono::system_clock::time_point start;

        // The end time
        const std::chrono::system_clock::time_point end;

        // The FIR for the event
        std::shared_ptr<const FlightInformationRegion::FlightInformationRegion> fir;

        // Code for the vatcan code
        std::string vatcanCode;

        // The participants
        std::vector<std::shared_ptr<EventParticipant>> participants;
    };
}// namespace ECFMP::Event
