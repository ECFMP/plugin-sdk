#pragma once
#include "euroscope/EuroscopeAircraftFactory.h"
#include <gmock/gmock.h>

namespace EuroScopePlugIn {
    class CFlightPlan;
    class CRadarTarget;
}// namespace EuroScopePlugIn

namespace ECFMPTest::Euroscope {
    class MockEuroscopeAircraftFactory : public ECFMP::Euroscope::EuroscopeAircraftFactory
    {
        public:
        MOCK_METHOD(
                std::shared_ptr<ECFMP::Euroscope::EuroscopeAircraft>, Make,
                (const EuroScopePlugIn::CFlightPlan&, const EuroScopePlugIn::CRadarTarget&), (const, noexcept, override)
        );
    };
}// namespace ECFMPTest::Euroscope
