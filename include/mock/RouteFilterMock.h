#pragma once
#include "../ECFMP/RouteFilter.h"
#include <gmock/gmock.h>

namespace ECFMP::Mock::FlowMeasure {

    class RouteFilterMock : public ECFMP::FlowMeasure::RouteFilter
    {
        public:
        MOCK_METHOD(const std::set<std::string>&, RouteStrings, (), (const, noexcept, override));
    };

}// namespace ECFMP::Mock::FlowMeasure
