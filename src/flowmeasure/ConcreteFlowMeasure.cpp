#include "ConcreteFlowMeasure.h"
#include "Measure.h"

namespace FlowSdk::FlowMeasure {

    ConcreteFlowMeasure::ConcreteFlowMeasure(
            int id, std::shared_ptr<const Event::Event> event, std::string identifier, std::string reason,
            std::chrono::system_clock::time_point startTime, std::chrono::system_clock::time_point endTime,
            std::chrono::system_clock::time_point withdrawnTime, MeasureStatus status,
            const std::set<std::shared_ptr<FlightInformationRegion::FlightInformationRegion>>& notifiedFirs,
            std::unique_ptr<struct Measure> measure)
        : id(id), event(std::move(event)), identifier(std::move(identifier)), reason(std::move(reason)),
          startTime(startTime), endTime(endTime), withdrawnTime(withdrawnTime), status(status),
          notifiedFirs(notifiedFirs), measure(std::move(measure))
    {
        assert(this->measure && "Measure not set in ConcreteFlowMeasure");
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

    const std::set<std::shared_ptr<FlightInformationRegion::FlightInformationRegion>>
    ConcreteFlowMeasure::NotifiedFlightInformationRegions() const noexcept
    {
        return notifiedFirs;
    }

    const Measure& ConcreteFlowMeasure::Measure() const noexcept
    {
        return *measure;
    }
}// namespace FlowSdk::FlowMeasure
