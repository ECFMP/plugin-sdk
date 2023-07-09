#pragma once
#include "ECFMP/EventListenerFilter.h"
#include "flowmeasure/ConcreteMeasure.h"
#include <gmock/gmock.h>

namespace ECFMPTest::Plugin {
    template<typename... Types>
    class MockEventListenerFilter : public ECFMP::Plugin::EventListenerFilter<Types...>
    {
        public:
        MOCK_METHOD(bool, Passes, (const Types&...), (override));
    };
}// namespace ECFMPTest::Plugin
