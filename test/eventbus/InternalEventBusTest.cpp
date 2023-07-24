#include "ECFMP/eventbus/EventFilter.h"
#include "ECFMP/eventbus/EventListener.h"
#include "eventbus/InternalEventBusFactory.h"
#include "plugin/InternalSdkEvents.h"
#include <chrono>
#include <thread>

namespace ECFMPTest::EvenBus {

    class MockEventListener : public ECFMP::EventBus::EventListener<int>
    {
        public:
        explicit MockEventListener(int expectedEvent) : expectedEvent(expectedEvent)
        {}

        void OnEvent(const int& event) override
        {
            callCount++;
            EXPECT_EQ(event, expectedEvent);
        }

        int expectedEvent;
        int callCount = 0;
    };

    class MockEventFilter : public ECFMP::EventBus::EventFilter<int>
    {
        public:
        explicit MockEventFilter(int expectedEvent, bool shouldProcess)
            : expectedEvent(expectedEvent), shouldProcess(shouldProcess)
        {}

        bool ShouldProcess(const int& event) override
        {
            callCount++;
            EXPECT_EQ(event, expectedEvent);
            return shouldProcess;
        }

        int expectedEvent;
        int callCount = 0;
        bool shouldProcess;
    };

    class InternalEventBusTest : public ::testing::Test
    {
        public:
        InternalEventBusTest() : eventBus(ECFMP::EventBus::MakeEventBus())
        {}

        static void WaitForEventsToProcess()
        {
            const auto start = std::chrono::system_clock::now();
            while (std::chrono::system_clock::now() - start < std::chrono::seconds(2)) {
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
            }
        }

        std::shared_ptr<ECFMP::EventBus::InternalEventBus> eventBus;
    };

    TEST_F(InternalEventBusTest, OnEventWithNoSubscriptionsDoesNotThrow)
    {
        ASSERT_NO_THROW(eventBus->OnEvent<int>(1));
    }

    TEST_F(InternalEventBusTest, ItThrowsOnSubscribeEuroscopeIfListenerIsNull)
    {
        ASSERT_THROW(eventBus->Subscribe<int>(nullptr), std::invalid_argument);
    }

    TEST_F(InternalEventBusTest, ItThrowsOnSubscribeOnceEuroscopeIfListenerIsNull)
    {
        ASSERT_THROW(eventBus->SubscribeOnce<int>(nullptr), std::invalid_argument);
    }

    TEST_F(InternalEventBusTest, ItSubscribesAListenerEuroscopeForMultipleCalls)
    {
        auto listener = std::make_shared<MockEventListener>(1);
        eventBus->Subscribe<int>(listener);
        eventBus->OnEvent<int>(1);
        eventBus->OnEvent<int>(1);
        eventBus->OnEvent<int>(1);
        eventBus->OnEvent<ECFMP::Plugin::EuroscopeTimerTickEvent>({});
        EXPECT_EQ(listener->callCount, 3);
    }

    TEST_F(InternalEventBusTest, ItSubscribesAListenerEuroscopeWithFilterForMultipleCalls)
    {
        auto listener = std::make_shared<MockEventListener>(1);
        auto filter = std::make_shared<MockEventFilter>(1, true);
        eventBus->Subscribe<int>(listener, filter);
        eventBus->OnEvent<int>(1);
        eventBus->OnEvent<int>(1);
        eventBus->OnEvent<int>(1);
        eventBus->OnEvent<ECFMP::Plugin::EuroscopeTimerTickEvent>({});
        EXPECT_EQ(listener->callCount, 3);
        EXPECT_EQ(filter->callCount, 3);
    }

    TEST_F(InternalEventBusTest, ItSubscribesAListenerEuroscopeOnce)
    {
        auto listener = std::make_shared<MockEventListener>(1);
        eventBus->SubscribeOnce<int>(listener);
        eventBus->OnEvent<int>(1);
        eventBus->OnEvent<int>(1);
        eventBus->OnEvent<int>(1);
        eventBus->OnEvent<ECFMP::Plugin::EuroscopeTimerTickEvent>({});
        EXPECT_EQ(listener->callCount, 1);
    }

    TEST_F(InternalEventBusTest, ItSubscribesAListenerEuroscopeOnceWithFilter)
    {
        auto listener = std::make_shared<MockEventListener>(1);
        auto filter = std::make_shared<MockEventFilter>(1, true);
        eventBus->SubscribeOnce<int>(listener, filter);
        eventBus->OnEvent<int>(1);
        eventBus->OnEvent<int>(1);
        eventBus->OnEvent<int>(1);
        eventBus->OnEvent<ECFMP::Plugin::EuroscopeTimerTickEvent>({});
        EXPECT_EQ(listener->callCount, 1);
        EXPECT_EQ(filter->callCount, 1);
    }

    TEST_F(InternalEventBusTest, ItThrowsOnSubscribeSyncIfListenerIsNull)
    {
        ASSERT_THROW(eventBus->SubscribeSync<int>(nullptr), std::invalid_argument);
    }

    TEST_F(InternalEventBusTest, ItThrowsOnSubscribeOnceSyncIfListenerIsNull)
    {
        ASSERT_THROW(eventBus->SubscribeSyncOnce<int>(nullptr), std::invalid_argument);
    }

    TEST_F(InternalEventBusTest, ItSubscribesAListenerSyncForMultipleCalls)
    {
        auto listener = std::make_shared<MockEventListener>(1);
        eventBus->SubscribeSync<int>(listener);
        eventBus->OnEvent<int>(1);
        eventBus->OnEvent<int>(1);
        eventBus->OnEvent<int>(1);
        EXPECT_EQ(listener->callCount, 3);
    }

    TEST_F(InternalEventBusTest, ItSubscribesAListenerSyncWithFilterForMultipleCalls)
    {
        auto listener = std::make_shared<MockEventListener>(1);
        auto filter = std::make_shared<MockEventFilter>(1, true);
        eventBus->SubscribeSync<int>(listener, filter);
        eventBus->OnEvent<int>(1);
        eventBus->OnEvent<int>(1);
        eventBus->OnEvent<int>(1);
        EXPECT_EQ(listener->callCount, 3);
        EXPECT_EQ(filter->callCount, 3);
    }

    TEST_F(InternalEventBusTest, ItSubscribesAListenerSyncOnce)
    {
        auto listener = std::make_shared<MockEventListener>(1);
        eventBus->SubscribeSyncOnce<int>(listener);
        eventBus->OnEvent<int>(1);
        eventBus->OnEvent<int>(1);
        eventBus->OnEvent<int>(1);
        EXPECT_EQ(listener->callCount, 1);
    }

    TEST_F(InternalEventBusTest, ItSubscribesAListenerSyncOnceWithFilter)
    {
        auto listener = std::make_shared<MockEventListener>(1);
        auto filter = std::make_shared<MockEventFilter>(1, true);
        eventBus->SubscribeSyncOnce<int>(listener, filter);
        eventBus->OnEvent<int>(1);
        eventBus->OnEvent<int>(1);
        eventBus->OnEvent<int>(1);
        EXPECT_EQ(listener->callCount, 1);
        EXPECT_EQ(filter->callCount, 1);
    }

    TEST_F(InternalEventBusTest, ItThrowsOnSubscribeAsyncIfListenerIsNull)
    {
        ASSERT_THROW(eventBus->SubscribeAsync<int>(nullptr), std::invalid_argument);
    }

    TEST_F(InternalEventBusTest, ItThrowsOnSubscribeOnceAsyncIfListenerIsNull)
    {
        ASSERT_THROW(eventBus->SubscribeAsyncOnce<int>(nullptr), std::invalid_argument);
    }

    TEST_F(InternalEventBusTest, ItSubscribesAListenerAsyncForMultipleCalls)
    {
        auto listener = std::make_shared<MockEventListener>(1);
        eventBus->SubscribeAsync<int>(listener);
        eventBus->OnEvent<int>(1);
        eventBus->OnEvent<int>(1);
        eventBus->OnEvent<int>(1);

        WaitForEventsToProcess();
        EXPECT_EQ(listener->callCount, 3);
    }

    TEST_F(InternalEventBusTest, ItSubscribesAListenerAsyncWithFilterForMultipleCalls)
    {
        auto listener = std::make_shared<MockEventListener>(1);
        auto filter = std::make_shared<MockEventFilter>(1, true);
        eventBus->SubscribeAsync<int>(listener, filter);
        eventBus->OnEvent<int>(1);
        eventBus->OnEvent<int>(1);
        eventBus->OnEvent<int>(1);

        WaitForEventsToProcess();
        EXPECT_EQ(listener->callCount, 3);
        EXPECT_EQ(filter->callCount, 3);
    }

    TEST_F(InternalEventBusTest, ItSubscribesAListenerAsyncOnce)
    {
        auto listener = std::make_shared<MockEventListener>(1);
        eventBus->SubscribeAsyncOnce<int>(listener);
        eventBus->OnEvent<int>(1);
        eventBus->OnEvent<int>(1);
        eventBus->OnEvent<int>(1);

        WaitForEventsToProcess();
        EXPECT_EQ(listener->callCount, 1);
    }

    TEST_F(InternalEventBusTest, ItSubscribesAListenerAsyncOnceWithFilter)
    {
        auto listener = std::make_shared<MockEventListener>(1);
        auto filter = std::make_shared<MockEventFilter>(1, true);
        eventBus->SubscribeSyncOnce<int>(listener, filter);
        eventBus->OnEvent<int>(1);
        eventBus->OnEvent<int>(1);
        eventBus->OnEvent<int>(1);

        WaitForEventsToProcess();
        EXPECT_EQ(listener->callCount, 1);
        EXPECT_EQ(filter->callCount, 1);
    }
}// namespace ECFMPTest::EvenBus
