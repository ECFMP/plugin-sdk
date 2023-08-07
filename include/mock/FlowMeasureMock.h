#pragma once
#include "ECFMP/flowmeasure/FlowMeasure.h"
#include <gmock/gmock.h>

namespace ECFMP::Mock::FlowMeasure {

    class FlowMeasureMock : public ECFMP::FlowMeasure::FlowMeasure
    {
        public:
        MOCK_METHOD(int, Id, (), (const, noexcept, override));
        MOCK_METHOD(std::shared_ptr<const ECFMP::Event::Event>, Event, (), (const, noexcept, override));
        MOCK_METHOD(const std::string&, Identifier, (), (const, noexcept, override));
        MOCK_METHOD(const std::string&, Reason, (), (const, noexcept, override));
        MOCK_METHOD(
                const ECFMP::FlowMeasure::CanonicalFlowMeasureInfo&, CanonicalInformation, (),
                (const, noexcept, override)
        );
        MOCK_METHOD(const std::chrono::system_clock::time_point&, StartTime, (), (const, noexcept, override));
        MOCK_METHOD(const std::chrono::system_clock::time_point&, EndTime, (), (const, noexcept, override));
        MOCK_METHOD(const std::chrono::system_clock::time_point&, WithdrawnAt, (), (const, override));
        MOCK_METHOD(ECFMP::FlowMeasure::MeasureStatus, Status, (), (const, noexcept, override));
        MOCK_METHOD(bool, HasStatus, (ECFMP::FlowMeasure::MeasureStatus), (const, noexcept, override));
        MOCK_METHOD(
                const std::vector<std::shared_ptr<const ECFMP::FlightInformationRegion::FlightInformationRegion>>,
                NotifiedFlightInformationRegions, (), (const, noexcept, override)
        );
        MOCK_METHOD(const ECFMP::FlowMeasure::Measure&, Measure, (), (const, noexcept, override));
        MOCK_METHOD(const ECFMP::FlowMeasure::FlowMeasureFilters&, Filters, (), (const, noexcept, override));
        MOCK_METHOD(
                bool, IsApplicableToFlightInformationRegion,
                (const ECFMP::FlightInformationRegion::FlightInformationRegion&), (const, noexcept, override)
        );
        MOCK_METHOD(bool, IsApplicableToFlightInformationRegion, (const std::string&), (const, noexcept, override));
        MOCK_METHOD(
                bool, ApplicableToAircraft, (const EuroScopePlugIn::CFlightPlan&, const EuroScopePlugIn::CRadarTarget&),
                (const, override)
        );
    };

}// namespace ECFMP::Mock::FlowMeasure
