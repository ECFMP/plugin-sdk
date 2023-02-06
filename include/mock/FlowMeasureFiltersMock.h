#pragma once
#include "../flow-sdk/FlowMeasureFilters.h"
#include <gmock/gmock.h>

namespace FlowSdk::Mock::FlowMeasure {

    class FlowMeasureFiltersMock : public FlowSdk::FlowMeasure::FlowMeasureFilters
    {
        public:
        MOCK_METHOD(bool, ApplicableToAirport, (const std::string&), (const, noexcept, override));
        MOCK_METHOD(bool, ApplicableToFlightInformationRegion, (const std::string&), (const, noexcept, override));
        MOCK_METHOD(bool, ApplicableToFlightInformationRegion, (int), (const, noexcept, override));
        MOCK_METHOD(bool, ApplicableToFlightInformationRegion,
                    (const FlowSdk::FlightInformationRegion::FlightInformationRegion&), (const, noexcept, override));
        MOCK_METHOD(void, ForEachAirportFilter,
                    (const std::function<void(const FlowSdk::FlowMeasure::AirportFilter&)>&),
                    (const, noexcept, override));
        MOCK_METHOD(void, ForEachEventFilter, (const std::function<void(const FlowSdk::FlowMeasure::EventFilter&)>&),
                    (const, noexcept, override));
        MOCK_METHOD(void, ForEachLevelFilter, (const std::function<void(const FlowSdk::FlowMeasure::LevelFilter&)>&),
                    (const, noexcept, override));
        MOCK_METHOD(void, ForEachRouteFilter, (const std::function<void(const FlowSdk::FlowMeasure::RouteFilter&)>&),
                    (const, noexcept, override));
        MOCK_METHOD(std::shared_ptr<FlowSdk::FlowMeasure::AirportFilter>, FirstAirportFilter,
                    (const std::function<bool(const FlowSdk::FlowMeasure::AirportFilter&)>&),
                    (const, noexcept, override));
        MOCK_METHOD(std::shared_ptr<FlowSdk::FlowMeasure::EventFilter>, FirstEventFilter,
                    (const std::function<bool(const FlowSdk::FlowMeasure::EventFilter&)>&),
                    (const, noexcept, override));
        MOCK_METHOD(std::shared_ptr<FlowSdk::FlowMeasure::LevelFilter>, FirstLevelFilter,
                    (const std::function<bool(const FlowSdk::FlowMeasure::LevelFilter&)>&),
                    (const, noexcept, override));
        MOCK_METHOD(std::shared_ptr<FlowSdk::FlowMeasure::RouteFilter>, FirstRouteFilter,
                    (const std::function<bool(const FlowSdk::FlowMeasure::RouteFilter&)>&),
                    (const, noexcept, override));
    };

}// namespace FlowSdk::Mock::FlowMeasure
