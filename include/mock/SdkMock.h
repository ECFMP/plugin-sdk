#pragma once
#include "../../src/eventbus/InternalEventBusFactory.h"
#include "ECFMP/Sdk.h"
#include "ECFMP/eventbus/EventBus.h"
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

        [[nodiscard]] auto EventBus() const noexcept -> ECFMP::EventBus::EventBus& override
        {
            if (!eventBus) {
                eventBus = ECFMP::EventBus::MakeEventBus();
            }

            return *eventBus;
        }

        MOCK_METHOD(void, OnEuroscopeTimerTick, (), (noexcept, override));
        MOCK_METHOD(void, Destroy, (), (noexcept, override));

        private:
        mutable std::shared_ptr<EventBus::EventBus> eventBus;
    };
}// namespace ECFMP::Mock::Plugin
