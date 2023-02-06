#pragma once
#include "../flow-sdk/RouteFilter.h"
#include <gmock/gmock.h>

namespace FlowSdk::Mock::FlowMeasure {

    class RouteFilterMock : public FlowSdk::FlowMeasure::RouteFilter
    {
        public:
        MOCK_METHOD(const std::set<std::string>&, RouteStrings, (), (const, noexcept, override));
    };

}// namespace FlowSdk::Mock::FlowMeasure
