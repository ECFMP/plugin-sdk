#pragma once
#include "../flow-sdk/FlowMeasure.h"
#include <gmock/gmock.h>

namespace FlowSdk::Mock::FlowMeasure {

    class FlowMeasureMock : public FlowSdk::FlowMeasure::FlowMeasure
    {
        public:
        MOCK_METHOD(int, Id, (), (const, noexcept, override));
        MOCK_METHOD(std::shared_ptr<const FlowSdk::Event::Event>, Event, (), (const, noexcept, override));
        MOCK_METHOD(const std::string&, Identifier, (), (const, noexcept, override));
        MOCK_METHOD(const std::string&, Reason, (), (const, noexcept, override));
        MOCK_METHOD(const std::chrono::system_clock::time_point&, StartTime, (), (const, noexcept, override));
        MOCK_METHOD(const std::chrono::system_clock::time_point&, EndTime, (), (const, noexcept, override));
        MOCK_METHOD(const std::chrono::system_clock::time_point&, WithdrawnAt, (), (const, override));
        MOCK_METHOD(FlowSdk::FlowMeasure::MeasureStatus, Status, (), (const, noexcept, override));
        MOCK_METHOD(bool, HasStatus, (FlowSdk::FlowMeasure::MeasureStatus), (const, noexcept, override));
        MOCK_METHOD(
                const std::vector<std::shared_ptr<const FlowSdk::FlightInformationRegion::FlightInformationRegion>>,
                NotifiedFlightInformationRegions, (), (const, noexcept, override)
        );
        MOCK_METHOD(const FlowSdk::FlowMeasure::Measure&, Measure, (), (const, noexcept, override));
        MOCK_METHOD(const FlowSdk::FlowMeasure::FlowMeasureFilters&, Filters, (), (const, noexcept, override));
    };

}// namespace FlowSdk::Mock::FlowMeasure
