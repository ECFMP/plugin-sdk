#pragma once
#include "euroscope/EuroscopeAircraft.h"
#include <gmock/gmock.h>

namespace ECFMPTest::Euroscope {

    class MockEuroscopeAircraft : public ECFMP::Euroscope::EuroscopeAircraft
    {
        public:
        MOCK_METHOD(int, Cid, (), (const, override));
        MOCK_METHOD(int, CurrentAltitudeStandardPressure, (), (const, override));
        MOCK_METHOD(int, CruiseAltitude, (), (const, override));
        MOCK_METHOD(int, GroundSpeed, (), (const, override));
        MOCK_METHOD(std::string, DepartureAirport, (), (const, override));
        MOCK_METHOD(std::string, DestinationAirport, (), (const, override));
        MOCK_METHOD(double, RangeToDestination, (), (const, override));
        MOCK_METHOD(std::string, RouteString, (), (const, override));
    };
}// namespace ECFMPTest::Euroscope
