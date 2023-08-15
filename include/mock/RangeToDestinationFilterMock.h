#pragma once
#include "ECFMP/flowmeasure/RangeToDestinationFilter.h"
#include <gmock/gmock.h>

namespace ECFMP::Mock::FlowMeasure {
    class RangeToDestinationFilterMock : public ECFMP::FlowMeasure::RangeToDestinationFilter
    {
        public:
        MOCK_METHOD(
                bool, ApplicableToAircraft, (const ECFMP::Euroscope::EuroscopeAircraft&), (const, noexcept, override)
        );
        MOCK_METHOD(int, Range, (), (const, noexcept, override));
        MOCK_METHOD(std::string, FilterDescription, (), (const, noexcept, override));
    };
}// namespace ECFMP::Mock::FlowMeasure
