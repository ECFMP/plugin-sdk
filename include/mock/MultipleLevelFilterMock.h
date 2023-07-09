#pragma once
#include "../ECFMP/MultipleLevelFilter.h"
#include <gmock/gmock.h>

namespace ECFMP::Mock::FlowMeasure {

    class LevelRangeFilterMock : public ECFMP::FlowMeasure::MultipleLevelFilter
    {
        public:
        MOCK_METHOD(std::vector<int>, Levels, (), (const, noexcept, override));
        MOCK_METHOD(std::vector<int>, Altitudes, (), (const, noexcept, override));
        MOCK_METHOD(bool, ApplicableToAltitude, (int), (const, noexcept, override));
        MOCK_METHOD(bool, ApplicableToLevel, (int), (const, noexcept, override));
    };

}// namespace ECFMP::Mock::FlowMeasure
