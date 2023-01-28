#pragma once
#include "Event.h"

namespace FlowSdk::FlightInformationRegion {
    class FlightInformationRegion;
}// namespace FlowSdk::FlightInformationRegion

namespace FlowSdk::Event {
    class ConcreteEvent : public Event::Event
    {
        public:
        ConcreteEvent(int id, const char*, std::chrono::system_clock::time_point start,
                      std::chrono::system_clock::time_point end,
                      std::shared_ptr<FlightInformationRegion::FlightInformationRegion> fir, const char* vatcanCode);
        [[nodiscard]] auto Id() const noexcept -> int override;
        [[nodiscard]] auto Name() const noexcept -> const char* override;
        [[nodiscard]] auto Start() const noexcept -> unsigned int override;
        [[nodiscard]] auto End() const noexcept -> unsigned int override;
        [[nodiscard]] auto FlightInformationRegion() const noexcept
                -> const FlightInformationRegion::FlightInformationRegion& override;
        [[nodiscard]] auto VatcanCode() const noexcept -> const char* override;

        private:
        // Api event id
        const int id;

        // Event name
        const char* name;

        // The start time
        const std::chrono::system_clock::time_point start;

        // The end time
        const std::chrono::system_clock::time_point end;

        // The FIR for the event
        std::shared_ptr<FlightInformationRegion::FlightInformationRegion> fir;

        // Code for the vatcan code
        const char* vatcanCode;
    };
}// namespace FlowSdk::Event
