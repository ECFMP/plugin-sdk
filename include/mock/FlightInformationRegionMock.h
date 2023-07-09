#pragma once
#include "../ECFMP/FlightInformationRegion.h"
#include <gmock/gmock.h>

namespace ECFMP::Mock::FlightInformationRegion {

    class FlightInformationRegionMock : public ECFMP::FlightInformationRegion::FlightInformationRegion
    {
        public:
        MOCK_METHOD(int, Id, (), (const, noexcept, override));
        MOCK_METHOD(const std::string&, Identifier, (), (const, noexcept, override));
        MOCK_METHOD(const std::string&, Name, (), (const, noexcept, override));
    };

}// namespace ECFMP::Mock::FlightInformationRegion
