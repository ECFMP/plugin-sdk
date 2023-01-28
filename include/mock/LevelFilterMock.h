#pragma once
#include "../flow-sdk/LevelFilter.h"
#include <gmock/gmock.h>

namespace FlowSdk::Mock::FlowMeasure {

    class LevelFilterMock : public FlowSdk::FlowMeasure::LevelFilter
    {
        public:
        MOCK_METHOD(FlowSdk::FlowMeasure::LevelFilterType, Type, (), (const, noexcept, override));
        MOCK_METHOD(int, Level, (), (const, noexcept, override));
        MOCK_METHOD(int, Altitude, (), (const, noexcept, override));
        MOCK_METHOD(bool, ApplicableToAltitude, (int), (const, noexcept, override));
        MOCK_METHOD(bool, ApplicableToLevel, (int), (const, noexcept, override));
    };

}// namespace FlowSdk::Mock::FlowMeasure
