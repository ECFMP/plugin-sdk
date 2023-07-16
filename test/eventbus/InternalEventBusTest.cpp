#include "eventbus/InternalEventBus.h"
#include "ECFMP/eventbus/EventFilter.h"
#include "ECFMP/eventbus/EventListener.h"

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
        ECFMP::EventBus::InternalEventBus eventBus;
    };

    TEST_F(InternalEventBusTest, OnEventWithNoSubscriptionsDoesNotThrow)
    {
        ASSERT_NO_THROW(eventBus.OnEvent<int>(1));
    }

    TEST_F(InternalEventBusTest, ItThrowsOnSubscribeIfListenerIsNull)
    {
        ASSERT_THROW(eventBus.Subscribe<int>(nullptr), std::invalid_argument);
    }

    TEST_F(InternalEventBusTest, ItThrowsOnSubscribeOnceIfListenerIsNull)
    {
        ASSERT_THROW(eventBus.SubscribeOnce<int>(nullptr), std::invalid_argument);
    }

    TEST_F(InternalEventBusTest, ItSubscribesAListenerForMultipleCalls)
    {
        auto listener = std::make_shared<MockEventListener>(1);
        eventBus.Subscribe<int>(listener);
        eventBus.OnEvent<int>(1);
        eventBus.OnEvent<int>(1);
        eventBus.OnEvent<int>(1);
        eventBus.ProcessPendingEvents();
        EXPECT_EQ(listener->callCount, 3);
    }

    TEST_F(InternalEventBusTest, ItSubscribesAListenerWithFilterForMultipleCalls)
    {
        auto listener = std::make_shared<MockEventListener>(1);
        auto filter = std::make_shared<MockEventFilter>(1, true);
        eventBus.Subscribe<int>(listener, filter);
        eventBus.OnEvent<int>(1);
        eventBus.OnEvent<int>(1);
        eventBus.OnEvent<int>(1);
        eventBus.ProcessPendingEvents();
        EXPECT_EQ(listener->callCount, 3);
        EXPECT_EQ(filter->callCount, 3);
    }

    TEST_F(InternalEventBusTest, ItSubscribesAListenerOnce)
    {
        auto listener = std::make_shared<MockEventListener>(1);
        eventBus.SubscribeOnce<int>(listener);
        eventBus.OnEvent<int>(1);
        eventBus.OnEvent<int>(1);
        eventBus.OnEvent<int>(1);
        eventBus.ProcessPendingEvents();
        EXPECT_EQ(listener->callCount, 1);
    }

    TEST_F(InternalEventBusTest, ItSubscribesAListenerOnceWithFilter)
    {
        auto listener = std::make_shared<MockEventListener>(1);
        auto filter = std::make_shared<MockEventFilter>(1, true);
        eventBus.SubscribeOnce<int>(listener, filter);
        eventBus.OnEvent<int>(1);
        eventBus.OnEvent<int>(1);
        eventBus.OnEvent<int>(1);
        eventBus.ProcessPendingEvents();
        EXPECT_EQ(listener->callCount, 1);
        EXPECT_EQ(filter->callCount, 1);
    }
}// namespace ECFMPTest::EvenBus
