#pragma once
#include "ECFMP/flowmeasure/Measure.h"
#include <gmock/gmock.h>

namespace ECFMP::Mock::FlowMeasure {

    class MeasureMock : public ECFMP::FlowMeasure::Measure
    {
        public:
        MOCK_METHOD(ECFMP::FlowMeasure::MeasureType, Type, (), (const, noexcept, override));
        MOCK_METHOD(int, IntegerValue, (), (const, override));
        MOCK_METHOD(double, DoubleValue, (), (const, override));
        MOCK_METHOD(const std::set<std::string>&, SetValue, (), (const, override));
        MOCK_METHOD(std::string, MeasureDescription, (), (const, noexcept, override));
    };

}// namespace ECFMP::Mock::FlowMeasure
