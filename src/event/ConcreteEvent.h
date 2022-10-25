#pragma once
#include "Event.h"

namespace FlowSdk::FlightInformationRegion {
    class FlightInformationRegion;
}// namespace FlowSdk::FlightInformationRegion

namespace FlowSdk::Event {
    class ConcreteEvent : public Event
    {
        public:
        ConcreteEvent(int id, std::string name, std::chrono::system_clock::time_point start,
                      std::chrono::system_clock::time_point end,
                      std::shared_ptr<FlightInformationRegion::FlightInformationRegion> fir);
        [[nodiscard]] auto Id() const -> int override;
        [[nodiscard]] auto Name() const -> const std::string& override;
        [[nodiscard]] auto Start() const -> const std::chrono::system_clock::time_point& override;
        [[nodiscard]] auto End() const -> const std::chrono::system_clock::time_point& override;
        [[nodiscard]] auto FlightInformationRegion() const
                -> const FlightInformationRegion::FlightInformationRegion& override;

        private:
        // Api event id
        const int id;

        // Event name
        const std::string name;

        // The start time
        const std::chrono::system_clock::time_point start;

        // The end time
        const std::chrono::system_clock::time_point end;

        // The FIR for the event
        std::shared_ptr<FlightInformationRegion::FlightInformationRegion> fir;
    };
}// namespace FlowSdk::Event
