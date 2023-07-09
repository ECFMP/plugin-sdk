#pragma once
#include "ECFMP/flowmeasure/FlowMeasureFilters.h"
#include "ECFMP/flowmeasure/MultipleLevelFilter.h"
#include <gmock/gmock.h>

namespace ECFMP::Mock::FlowMeasure {

    class FlowMeasureFiltersMock : public ECFMP::FlowMeasure::FlowMeasureFilters
    {
        public:
        MOCK_METHOD(bool, ApplicableToAirport, (const std::string&), (const, noexcept, override));
        MOCK_METHOD(
                void, ForEachAirportFilter, (const std::function<void(const ECFMP::FlowMeasure::AirportFilter&)>&),
                (const, noexcept, override)
        );
        MOCK_METHOD(
                void, ForEachEventFilter, (const std::function<void(const ECFMP::FlowMeasure::EventFilter&)>&),
                (const, noexcept, override)
        );
        MOCK_METHOD(
                void, ForEachLevelFilter, (const std::function<void(const ECFMP::FlowMeasure::LevelRangeFilter&)>&),
                (const, noexcept, override)
        );
        MOCK_METHOD(
                void, ForEachMultipleLevelFilter,
                (const std::function<void(const ECFMP::FlowMeasure::MultipleLevelFilter&)>&),
                (const, noexcept, override)
        );
        MOCK_METHOD(
                void, ForEachRouteFilter, (const std::function<void(const ECFMP::FlowMeasure::RouteFilter&)>&),
                (const, noexcept, override)
        );
        MOCK_METHOD(
                void, ForEachRangeToDestinationFilter,
                (const std::function<void(const ECFMP::FlowMeasure::RangeToDestinationFilter&)>&),
                (const, noexcept, override)
        );
        MOCK_METHOD(
                std::shared_ptr<ECFMP::FlowMeasure::AirportFilter>, FirstAirportFilter,
                (const std::function<bool(const ECFMP::FlowMeasure::AirportFilter&)>&), (const, noexcept, override)
        );
        MOCK_METHOD(
                std::shared_ptr<ECFMP::FlowMeasure::EventFilter>, FirstEventFilter,
                (const std::function<bool(const ECFMP::FlowMeasure::EventFilter&)>&), (const, noexcept, override)
        );
        MOCK_METHOD(
                std::shared_ptr<ECFMP::FlowMeasure::LevelRangeFilter>, FirstLevelFilter,
                (const std::function<bool(const ECFMP::FlowMeasure::LevelRangeFilter&)>&), (const, noexcept, override)
        );
        MOCK_METHOD(
                std::shared_ptr<ECFMP::FlowMeasure::MultipleLevelFilter>, FirstMultipleLevelFilter,
                (const std::function<bool(const ECFMP::FlowMeasure::MultipleLevelFilter&)>&),
                (const, noexcept, override)
        );
        MOCK_METHOD(
                std::shared_ptr<ECFMP::FlowMeasure::RouteFilter>, FirstRouteFilter,
                (const std::function<bool(const ECFMP::FlowMeasure::RouteFilter&)>&), (const, noexcept, override)
        );
        MOCK_METHOD(
                std::shared_ptr<ECFMP::FlowMeasure::RangeToDestinationFilter>, FirstRangeToDestinationFilter,
                (const std::function<bool(const ECFMP::FlowMeasure::RangeToDestinationFilter&)>&),
                (const, noexcept, override)
        );
    };

}// namespace ECFMP::Mock::FlowMeasure
