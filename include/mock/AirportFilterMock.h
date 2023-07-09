#pragma once
#include "../ECFMP/AirportFilter.h"
#include <gmock/gmock.h>

namespace ECFMP::Mock::FlowMeasure {

    class AirportFilterMock : public ECFMP::FlowMeasure::AirportFilter
    {
        public:
        MOCK_METHOD(const std::set<std::string>&, AirportStrings, (), (const, noexcept, override));
        MOCK_METHOD(ECFMP::FlowMeasure::AirportFilterType, Type, (), (const, noexcept, override));
        MOCK_METHOD(bool, ApplicableToAirport, (const std::string&), (const, noexcept, override));
    };

}// namespace ECFMP::Mock::FlowMeasure
