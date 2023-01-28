#pragma once
#include "../flow-sdk/AirportFilter.h"
#include <gmock/gmock.h>

namespace FlowSdk::Mock::FlowMeasure {

    class AirportFilterMock : public FlowSdk::FlowMeasure::AirportFilter
    {
        public:
        MOCK_METHOD(const std::set<std::string>&, AirportStrings, (), (const, noexcept, override));
        MOCK_METHOD(bool, ApplicableToAirport, (const std::string&), (const, noexcept, override));
    };

}// namespace FlowSdk::Mock::FlowMeasure
