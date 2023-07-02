#pragma once
#include "../flow-sdk/MultipleLevelFilter.h"
#include <gmock/gmock.h>

namespace FlowSdk::Mock::FlowMeasure {

    class LevelRangeFilterMock : public FlowSdk::FlowMeasure::MultipleLevelFilter
    {
        public:
        MOCK_METHOD(std::vector<int>, Levels, (), (const, noexcept, override));
        MOCK_METHOD(std::vector<int>, Altitudes, (), (const, noexcept, override));
        MOCK_METHOD(bool, ApplicableToAltitude, (int), (const, noexcept, override));
        MOCK_METHOD(bool, ApplicableToLevel, (int), (const, noexcept, override));
    };

}// namespace FlowSdk::Mock::FlowMeasure
