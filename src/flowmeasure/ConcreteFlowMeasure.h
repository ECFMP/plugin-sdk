#pragma once
#include "ECFMP/flowmeasure/FlowMeasure.h"

namespace ECFMP::FlightInformationRegion {
    class FlightInformationRegion;
}// namespace ECFMP::FlightInformationRegion

namespace ECFMP::FlowMeasure {

    class CustomFlowMeasureFilter;

    class ConcreteFlowMeasure : public FlowMeasure
    {
        public:
        ConcreteFlowMeasure(
                int id, std::shared_ptr<const Event::Event> event, std::string identifier, std::string reason,
                std::chrono::system_clock::time_point startTime, std::chrono::system_clock::time_point endTime,
                std::chrono::system_clock::time_point withdrawnTime, MeasureStatus status,
                const std::vector<std::shared_ptr<const FlightInformationRegion::FlightInformationRegion>> notifiedFirs,
                std::unique_ptr<class Measure> measure, std::unique_ptr<FlowMeasureFilters> filters,
                std::shared_ptr<std::vector<std::shared_ptr<ECFMP::FlowMeasure::CustomFlowMeasureFilter>>> customFilters
        );
        ~ConcreteFlowMeasure() override;
        [[nodiscard]] auto Id() const noexcept -> int override;
        [[nodiscard]] auto Event() const noexcept -> std::shared_ptr<const Event::Event> override;
        [[nodiscard]] auto Identifier() const noexcept -> const std::string& override;
        [[nodiscard]] auto CanonicalInformation() const noexcept -> const CanonicalFlowMeasureInfo& override;
        [[nodiscard]] auto Reason() const noexcept -> const std::string& override;
        [[nodiscard]] auto StartTime() const noexcept -> const std::chrono::system_clock::time_point& override;
        [[nodiscard]] auto EndTime() const noexcept -> const std::chrono::system_clock::time_point& override;
        [[nodiscard]] auto WithdrawnAt() const -> const std::chrono::system_clock::time_point& override;
        [[nodiscard]] auto Status() const noexcept -> MeasureStatus override;
        [[nodiscard]] auto HasStatus(MeasureStatus checkStatus) const noexcept -> bool override;
        [[nodiscard]] auto NotifiedFlightInformationRegions() const noexcept
                -> const std::vector<std::shared_ptr<const FlightInformationRegion::FlightInformationRegion>>& override;
        [[nodiscard]] auto Measure() const noexcept -> const class Measure& override;
        [[nodiscard]] auto Filters() const noexcept -> const FlowMeasureFilters& override;
        [[nodiscard]] auto IsApplicableToFlightInformationRegion(
                const FlightInformationRegion::FlightInformationRegion& flightInformationRegion
        ) const noexcept -> bool override;
        [[nodiscard]] auto IsApplicableToFlightInformationRegion(const std::string& flightInformationRegion
        ) const noexcept -> bool override;
        [[nodiscard]] auto ApplicableToAircraft(
                const EuroScopePlugIn::CFlightPlan& flightplan, const EuroScopePlugIn::CRadarTarget& radarTarget
        ) const -> bool override;

        private:
        // Id of the measure
        int id;

        // The event associated with the measure, if there is one
        std::shared_ptr<const Event::Event> event;

        // Identifier for the measure
        std::string identifier;

        // Canonical identifier for the measure
        std::unique_ptr<CanonicalFlowMeasureInfo> canonicalInformation;

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
        const std::vector<std::shared_ptr<const FlightInformationRegion::FlightInformationRegion>> notifiedFirs;

        // The measure itself
        std::unique_ptr<class Measure> measure;

        // The filters
        std::unique_ptr<FlowMeasureFilters> filters;

        // Custom filters
        std::shared_ptr<std::vector<std::shared_ptr<ECFMP::FlowMeasure::CustomFlowMeasureFilter>>> customFilters;
    };
}// namespace ECFMP::FlowMeasure
