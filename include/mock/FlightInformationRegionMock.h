#pragma once
#include "../flow-sdk/FlightInformationRegion.h"
#include <gmock/gmock.h>

namespace FlowSdk::Mock::FlightInformationRegion {

    class FlightInformationRegionMock : public FlowSdk::FlightInformationRegion::FlightInformationRegion
    {
        public:
        MOCK_METHOD(int, Id, (), (const, noexcept, override));
        MOCK_METHOD(const std::string&, Identifier, (), (const, noexcept, override));
        MOCK_METHOD(const std::string&, Name, (), (const, noexcept, override));
    };

}// namespace FlowSdk::Mock::FlightInformationRegion
