#pragma once
#include "flow-sdk/EventListener.h"
#include "flow-sdk/FlowMeasure.h"
#include <gmock/gmock.h>

namespace FlowSdkTest::Plugin {
    template<typename... Types>
    class MockEventListener : public FlowSdk::Plugin::EventListener<Types...>
    {
        public:
        MOCK_METHOD(void, OnEvent, (const Types&...), (override));
    };
}// namespace FlowSdkTest::Plugin
