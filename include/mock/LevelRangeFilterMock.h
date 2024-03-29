#pragma once
#include "ECFMP/flowmeasure/LevelRangeFilter.h"
#include <gmock/gmock.h>

namespace ECFMP::Mock::FlowMeasure {

    class LevelRangeFilterMock : public ECFMP::FlowMeasure::LevelRangeFilter
    {
        public:
        MOCK_METHOD(ECFMP::FlowMeasure::LevelRangeFilterType, Type, (), (const, noexcept, override));
        MOCK_METHOD(int, Level, (), (const, noexcept, override));
        MOCK_METHOD(int, Altitude, (), (const, noexcept, override));
        MOCK_METHOD(bool, ApplicableToAltitude, (int), (const, noexcept, override));
        MOCK_METHOD(bool, ApplicableToLevel, (int), (const, noexcept, override));
        MOCK_METHOD(bool, ApplicableToAircraft, (const Euroscope::EuroscopeAircraft&), (const, noexcept, override));
        MOCK_METHOD(std::string, FilterDescription, (), (const, noexcept, override));
    };

}// namespace ECFMP::Mock::FlowMeasure
