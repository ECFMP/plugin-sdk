#pragma once
#include "../flow-sdk/LevelRangeFilter.h"
#include <gmock/gmock.h>

namespace FlowSdk::Mock::FlowMeasure {

    class LevelRangeFilterMock : public FlowSdk::FlowMeasure::LevelRangeFilter
    {
        public:
        MOCK_METHOD(FlowSdk::FlowMeasure::LevelRangeFilterType, Type, (), (const, noexcept, override));
        MOCK_METHOD(int, Level, (), (const, noexcept, override));
        MOCK_METHOD(int, Altitude, (), (const, noexcept, override));
        MOCK_METHOD(bool, ApplicableToAltitude, (int), (const, noexcept, override));
        MOCK_METHOD(bool, ApplicableToLevel, (int), (const, noexcept, override));
    };

}// namespace FlowSdk::Mock::FlowMeasure
