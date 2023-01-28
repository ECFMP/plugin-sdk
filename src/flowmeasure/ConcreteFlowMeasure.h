#pragma once
#include "flow-sdk/FlowMeasure.h"

namespace FlowSdk::FlightInformationRegion {
    class FlightInformationRegion;
}// namespace FlowSdk::FlightInformationRegion

namespace FlowSdk::FlowMeasure {

    class ConcreteFlowMeasure : public FlowMeasure
    {
        public:
        ConcreteFlowMeasure(
                int id, std::shared_ptr<const Event::Event> event, std::string identifier, std::string reason,
                std::chrono::system_clock::time_point startTime, std::chrono::system_clock::time_point endTime,
                std::chrono::system_clock::time_point withdrawnTime, MeasureStatus status,
                const std::set<std::shared_ptr<FlightInformationRegion::FlightInformationRegion>>& notifiedFirs,
                std::unique_ptr<class Measure> measure);
        ~ConcreteFlowMeasure();
        auto Id() const noexcept -> int override;
        auto Event() const noexcept -> std::shared_ptr<const Event::Event> override;
        auto Identifier() const noexcept -> const std::string& override;
        auto Reason() const noexcept -> const std::string& override;
        auto StartTime() const noexcept -> const std::chrono::system_clock::time_point& override;
        auto EndTime() const noexcept -> const std::chrono::system_clock::time_point& override;
        auto WithdrawnAt() const -> const std::chrono::system_clock::time_point& override;
        auto Status() const noexcept -> MeasureStatus override;
        auto HasStatus(MeasureStatus checkStatus) const noexcept -> bool override;
        auto NotifiedFlightInformationRegions() const noexcept
                -> const std::set<std::shared_ptr<FlightInformationRegion::FlightInformationRegion>> override;
        auto Measure() const noexcept -> const class Measure& override;

        private:
        // Id of the measure
        int id;

        // The event associated with the measure, if there is one
        std::shared_ptr<const Event::Event> event;

        // Identifier for the measure
        std::string identifier;

        // Why the measure was enacted
        std::string reason;

        // The start time of the measure
        std::chrono::system_clock::time_point startTime;

        // End time of the measure
        std::chrono::system_clock::time_point endTime;

        // The time of withdrawl
        std::chrono::system_clock::time_point withdrawnTime;

        // Current measure status
        MeasureStatus status;

        // FIRs notified about this measure
        const std::set<std::shared_ptr<FlightInformationRegion::FlightInformationRegion>> notifiedFirs;

        // The measure itself
        std::unique_ptr<class Measure> measure;
    };

}// namespace FlowSdk::FlowMeasure
