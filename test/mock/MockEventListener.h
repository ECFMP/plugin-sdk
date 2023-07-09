#pragma once
#include "flow-sdk/EventListener.h"
#include "flow-sdk/FlowMeasure.h"
#include <gmock/gmock.h>

namespace ECFMPTest::Plugin {
    template<typename... Types>
    class MockEventListener : public ECFMP::Plugin::EventListener<Types...>
    {
        public:
        MOCK_METHOD(void, OnEvent, (const Types&...), (override));
    };
}// namespace ECFMPTest::Plugin
