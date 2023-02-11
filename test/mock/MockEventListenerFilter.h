#pragma once
#include "flow-sdk/EventListenerFilter.h"
#include "flowmeasure/ConcreteMeasure.h"
#include <gmock/gmock.h>

namespace FlowSdkTest::Plugin {
    template<typename... Types>
    class MockEventListenerFilter : public FlowSdk::Plugin::EventListenerFilter<Types...>
    {
        public:
        MOCK_METHOD(bool, Passes, (const Types&...), (override));
    };
}// namespace FlowSdkTest::Plugin
