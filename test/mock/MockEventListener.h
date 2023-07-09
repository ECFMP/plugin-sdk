#pragma once
#include "ECFMP/EventListener.h"
#include "ECFMP/FlowMeasure.h"
#include <gmock/gmock.h>

namespace ECFMPTest::Plugin {
    template<typename... Types>
    class MockEventListener : public ECFMP::Plugin::EventListener<Types...>
    {
        public:
        MOCK_METHOD(void, OnEvent, (const Types&...), (override));
    };
}// namespace ECFMPTest::Plugin
