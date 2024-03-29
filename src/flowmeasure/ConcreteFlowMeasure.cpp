#include "ConcreteFlowMeasure.h"
#include "ECFMP/flightinformationregion/FlightInformationRegion.h"
#include "ECFMP/flowmeasure/CanonicalFlowMeasureInfo.h"
#include "ECFMP/flowmeasure/CustomFlowMeasureFilter.h"
#include "ECFMP/flowmeasure/FlowMeasureFilters.h"
#include "ECFMP/flowmeasure/Measure.h"

namespace ECFMP::FlowMeasure {

    ConcreteFlowMeasure::ConcreteFlowMeasure(
            int id, std::shared_ptr<const Event::Event> event, std::string identifier, std::string reason,
            std::chrono::system_clock::time_point startTime, std::chrono::system_clock::time_point endTime,
            std::chrono::system_clock::time_point withdrawnTime, MeasureStatus status,
            const std::vector<std::shared_ptr<const FlightInformationRegion::FlightInformationRegion>> notifiedFirs,
            std::unique_ptr<class Measure> measure, std::unique_ptr<FlowMeasureFilters> filters,
            std::shared_ptr<std::vector<std::shared_ptr<ECFMP::FlowMeasure::CustomFlowMeasureFilter>>> customFilters
    )
        : id(id), event(std::move(event)), identifier(std::move(identifier)),
          canonicalInformation(std::make_unique<CanonicalFlowMeasureInfo>(this->identifier)), reason(std::move(reason)),
          startTime(startTime), endTime(endTime), withdrawnTime(withdrawnTime), status(status),
          notifiedFirs(notifiedFirs), measure(std::move(measure)), filters(std::move(filters)),
          customFilters(std::move(customFilters))
    {
        assert(this->measure && "Measure not set in ConcreteFlowMeasure");
        assert(this->filters && "Filters not set in ConcreteFlowMeasure");
        assert(this->customFilters && "Custom filters not set in ConcreteFlowMeasure");
    }

    ConcreteFlowMeasure::~ConcreteFlowMeasure() = default;

    int ConcreteFlowMeasure::Id() const noexcept
    {
        return id;
    }

    std::shared_ptr<const Event::Event> ConcreteFlowMeasure::Event() const noexcept
    {
        return event;
    }

    const std::string& ConcreteFlowMeasure::Identifier() const noexcept
    {
        return identifier;
    }

    const std::string& ConcreteFlowMeasure::Reason() const noexcept
    {
        return reason;
    }

    const std::chrono::system_clock::time_point& ConcreteFlowMeasure::StartTime() const noexcept
    {
        return startTime;
    }

    const std::chrono::system_clock::time_point& ConcreteFlowMeasure::EndTime() const noexcept
    {
        return endTime;
    }

    const std::chrono::system_clock::time_point& ConcreteFlowMeasure::WithdrawnAt() const
    {
        if (!HasStatus(MeasureStatus::Withdrawn)) {
            throw FlowMeasureNotWithdrawnException();
        }

        return withdrawnTime;
    }

    MeasureStatus ConcreteFlowMeasure::Status() const noexcept
    {
        return status;
    }

    bool ConcreteFlowMeasure::HasStatus(MeasureStatus checkStatus) const noexcept
    {
        return checkStatus == status;
    }

    auto ConcreteFlowMeasure::NotifiedFlightInformationRegions() const noexcept
            -> const std::vector<std::shared_ptr<const FlightInformationRegion::FlightInformationRegion>>&
    {
        return notifiedFirs;
    }

    const Measure& ConcreteFlowMeasure::Measure() const noexcept
    {
        return *measure;
    }

    auto ConcreteFlowMeasure::Filters() const noexcept -> const FlowMeasureFilters&
    {
        return *filters;
    }

    auto ConcreteFlowMeasure::IsApplicableToFlightInformationRegion(
            const FlightInformationRegion::FlightInformationRegion& flightInformationRegion
    ) const noexcept -> bool
    {
        return IsApplicableToFlightInformationRegion(flightInformationRegion.Identifier());
    }

    auto ConcreteFlowMeasure::IsApplicableToFlightInformationRegion(const std::string& flightInformationRegion
    ) const noexcept -> bool
    {
        return std::find_if(
                       notifiedFirs.begin(), notifiedFirs.end(),
                       [&flightInformationRegion](
                               const std::shared_ptr<const FlightInformationRegion::FlightInformationRegion>& fir
                       ) {
                           return fir->Identifier() == flightInformationRegion;
                       }
               ) != notifiedFirs.end();
    }

    auto ConcreteFlowMeasure::CanonicalInformation() const noexcept -> const CanonicalFlowMeasureInfo&
    {
        return *canonicalInformation;
    }

    bool ConcreteFlowMeasure::ApplicableToAircraft(
            const EuroScopePlugIn::CFlightPlan& flightplan, const EuroScopePlugIn::CRadarTarget& radarTarget
    ) const
    {
        return filters->ApplicableToAircraft(flightplan, radarTarget)
                && std::all_of(
                        customFilters->cbegin(), customFilters->cend(),
                        [&flightplan, &radarTarget, this](const std::shared_ptr<CustomFlowMeasureFilter>& filter) {
                            return filter->ApplicableToAircraft(flightplan, radarTarget, *this);
                        }
                );
    }
}// namespace ECFMP::FlowMeasure
