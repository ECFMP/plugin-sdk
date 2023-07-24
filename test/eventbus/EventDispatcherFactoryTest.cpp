#include "eventbus/EventDispatcherFactory.h"
#include "eventbus/PendingEuroscopeEvents.h"
#include "eventbus/SubscriptionFlags.h"

namespace ECFMPTest::EventBus {

    class MockEventDispatcherListener : public ECFMP::EventBus::EventListener<int>
    {
        public:
        explicit MockEventDispatcherListener(int expectedEvent) : expectedEvent(expectedEvent)
        {}

        void OnEvent(const int& event) override
        {
            callCount++;
            EXPECT_EQ(event, expectedEvent);
        }

        int expectedEvent;
        int callCount = 0;
    };

    class EventDispatcherFactoryTest : public testing::Test
    {
        public:
        EventDispatcherFactoryTest()
            : pendingEvents(std::make_shared<ECFMP::EventBus::PendingEuroscopeEvents>()),
              eventDispatcherFactory(std::make_shared<ECFMP::EventBus::EventDispatcherFactory>(pendingEvents))
        {}

        [[nodiscard]] static auto CreateListener() -> std::shared_ptr<MockEventDispatcherListener>
        {
            return std::make_shared<MockEventDispatcherListener>(5);
        }

        static void WaitForEventsToProcess()
        {
            const auto start = std::chrono::system_clock::now();
            while (std::chrono::system_clock::now() - start < std::chrono::seconds(2)) {
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
            }
        }

        std::shared_ptr<ECFMP::EventBus::PendingEuroscopeEvents> pendingEvents;
        std::shared_ptr<ECFMP::EventBus::EventDispatcherFactory> eventDispatcherFactory;
    };

    TEST_F(EventDispatcherFactoryTest, ItMakesASynchronousDispatcher)
    {
        auto listener = CreateListener();
        auto dispatcher =
                eventDispatcherFactory->CreateDispatcher<int>(listener, ECFMP::EventBus::EventDispatchMode::Sync);
        EXPECT_NE(nullptr, dispatcher);
        EXPECT_EQ(0, listener->callCount);
        dispatcher->Dispatch(5);
        EXPECT_EQ(1, listener->callCount);
    }

    TEST_F(EventDispatcherFactoryTest, ItMakesAEuroscopeDispatcher)
    {
        auto listener = CreateListener();
        auto dispatcher =
                eventDispatcherFactory->CreateDispatcher<int>(listener, ECFMP::EventBus::EventDispatchMode::Euroscope);
        EXPECT_NE(nullptr, dispatcher);
        EXPECT_EQ(0, listener->callCount);
        dispatcher->Dispatch(5);
        EXPECT_EQ(0, listener->callCount);
        pendingEvents->OnEvent({});
        EXPECT_EQ(1, listener->callCount);
    }

    TEST_F(EventDispatcherFactoryTest, ItMakesAnAsynchronousDispatcher)
    {
        auto listener = CreateListener();
        auto dispatcher =
                eventDispatcherFactory->CreateDispatcher<int>(listener, ECFMP::EventBus::EventDispatchMode::Async);
        EXPECT_NE(nullptr, dispatcher);
        EXPECT_EQ(0, listener->callCount);
        dispatcher->Dispatch(5);
        WaitForEventsToProcess();
        EXPECT_EQ(1, listener->callCount);
    }
}// namespace ECFMPTest::EventBus
