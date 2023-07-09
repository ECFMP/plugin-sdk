#include "plugin/ConcreteSdk.h"
#include "api/ApiDataDownloader.h"
#include "eventbus/InternalEventBus.h"
#include "flow-sdk/NewEventListener.h"
#include "mock/MockHttpClient.h"

namespace FlowSdkTest::Plugin {

    class MockEventListener : public FlowSdk::EventBus::NewEventListener<int>
    {
        public:
        void OnEvent(const int&) override
        {
            callCount++;
        }

        int callCount = 0;
    };

    class ConcreteSdkTest : public testing::Test
    {
        public:
        ConcreteSdkTest()
            : eventBus(std::make_shared<FlowSdk::EventBus::InternalEventBus>()), testPtr(std::make_shared<int>(5)),
              instance(std::shared_ptr<void>(testPtr), eventBus)
        {}

        std::shared_ptr<FlowSdk::EventBus::InternalEventBus> eventBus;
        std::shared_ptr<int> testPtr;
        FlowSdk::Plugin::ConcreteSdk instance;
    };

    TEST_F(ConcreteSdkTest, OnEuroscopeTimerTickProcessesEvents)
    {
        auto mockEventListener = std::make_shared<MockEventListener>();
        eventBus->Subscribe<int>(mockEventListener);
        eventBus->OnEvent<int>(5);
        EXPECT_EQ(0, mockEventListener->callCount);
        instance.OnEuroscopeTimerTick();
        EXPECT_EQ(1, mockEventListener->callCount);
    }

    TEST_F(ConcreteSdkTest, ItHasAnEventBus)
    {
        auto mockEventListener = std::make_shared<MockEventListener>();
        instance.EventBus().Subscribe<int>(mockEventListener);
        eventBus->OnEvent<int>(5);
        instance.OnEuroscopeTimerTick();
        EXPECT_EQ(1, mockEventListener->callCount);
    }
}// namespace FlowSdkTest::Plugin
