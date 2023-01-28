#pragma once
#include "../flow-sdk/Measure.h"
#include <gmock/gmock.h>

namespace FlowSdk::Mock::FlowMeasure {

    class MeasureMock : public FlowSdk::FlowMeasure::Measure
    {
        public:
        MOCK_METHOD(FlowSdk::FlowMeasure::MeasureType, Type, (), (const, noexcept, override));
        MOCK_METHOD(int, IntegerValue, (), (const, override));
        MOCK_METHOD(double, DoubleValue, (), (const, override));
        MOCK_METHOD(const std::set<std::string>&, SetValue, (), (const, override));
    };

}// namespace FlowSdk::Mock::FlowMeasure
