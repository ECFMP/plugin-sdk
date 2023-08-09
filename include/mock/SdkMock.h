#pragma once
#include "ECFMP/Sdk.h"
#include <gmock/gmock.h>

namespace ECFMP::Mock::Plugin {

    class SdkMock : public ECFMP::Plugin::Sdk
    {
        public:
        MOCK_METHOD(
                std::shared_ptr<const Api::FlightInformationRegionCollection>, FlightInformationRegions, (),
                (const, noexcept, override)
        );
        MOCK_METHOD(std::shared_ptr<const Api::EventCollection>, Events, (), (const, noexcept, override));
        MOCK_METHOD(std::shared_ptr<const Api::FlowMeasureCollection>, FlowMeasures, (), (const, noexcept, override));
        MOCK_METHOD(ECFMP::EventBus::EventBus&, EventBus, (), (const, noexcept, override));
        MOCK_METHOD(void, OnEuroscopeTimerTick, (), (noexcept, override));
        MOCK_METHOD(void, Destroy, (), (noexcept, override));
    };
}// namespace ECFMP::Mock::Plugin
