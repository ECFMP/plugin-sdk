#include "eventbus/EventStream.h"
#include "ECFMP/eventbus/EventListener.h"
#include "eventbus/SubscriptionFlags.h"
#include "eventbus/SynchronousEventDispatcher.h"

namespace ECFMP::EventBus {

    class MockEventListener : public EventListener<int>
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

    class MockEventFilter : public EventFilter<int>
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

    class EventStreamTest : public testing::Test
    {
        public:
        EventStream<int> eventStream;
    };

    TEST_F(EventStreamTest, OnEventWithNoSubscriptionsDoesNotThrow)
    {
        ASSERT_NO_THROW(eventStream.OnEvent(1));
    }

    TEST_F(EventStreamTest, OnEventWithOneSubscriptionCallsListener)
    {
        auto listener = std::make_shared<MockEventListener>(1);
        auto dispatcher = std::make_shared<SynchronousEventDispatcher<int>>(listener);
        eventStream.Subscribe({dispatcher, listener, nullptr, {ECFMP::EventBus::EventDispatchMode::Sync, false}});
        eventStream.OnEvent(1);

        EXPECT_EQ(1, listener->callCount);
    }

    TEST_F(EventStreamTest, OnEventWithOneSubscriptionCallsListenerMultipleTimes)
    {
        auto listener = std::make_shared<MockEventListener>(1);
        auto dispatcher = std::make_shared<SynchronousEventDispatcher<int>>(listener);
        eventStream.Subscribe({dispatcher, listener, nullptr, {ECFMP::EventBus::EventDispatchMode::Sync, false}});
        eventStream.OnEvent(1);
        eventStream.OnEvent(1);

        EXPECT_EQ(2, listener->callCount);
    }

    TEST_F(EventStreamTest, OnEventWithOneSubscriptionOnceCallsListenerOnce)
    {
        auto listener = std::make_shared<MockEventListener>(1);
        auto dispatcher = std::make_shared<SynchronousEventDispatcher<int>>(listener);
        eventStream.Subscribe({dispatcher, listener, nullptr, {ECFMP::EventBus::EventDispatchMode::Sync, true}});
        eventStream.OnEvent(1);
        eventStream.OnEvent(1);

        EXPECT_EQ(1, listener->callCount);
    }

    TEST_F(EventStreamTest, OnEventWithOneSubscriptionAndFilterCallsListener)
    {
        auto listener = std::make_shared<MockEventListener>(1);
        auto dispatcher = std::make_shared<SynchronousEventDispatcher<int>>(listener);
        auto filter = std::make_shared<MockEventFilter>(1, true);
        eventStream.Subscribe({dispatcher, listener, filter, {ECFMP::EventBus::EventDispatchMode::Sync, false}});
        eventStream.OnEvent(1);

        EXPECT_EQ(1, listener->callCount);
        EXPECT_EQ(1, filter->callCount);
    }

    TEST_F(EventStreamTest, OnEventWithOneSubscriptionAndFilterCallsListenerMultipleTimes)
    {
        auto listener = std::make_shared<MockEventListener>(1);
        auto dispatcher = std::make_shared<SynchronousEventDispatcher<int>>(listener);
        auto filter = std::make_shared<MockEventFilter>(1, true);
        eventStream.Subscribe({dispatcher, listener, filter, {ECFMP::EventBus::EventDispatchMode::Sync, false}});
        eventStream.OnEvent(1);
        eventStream.OnEvent(1);

        EXPECT_EQ(2, listener->callCount);
        EXPECT_EQ(2, filter->callCount);
    }

    TEST_F(EventStreamTest, OnEventWithOneSubscriptionAndFilterDoesNotCallListener)
    {
        auto listener = std::make_shared<MockEventListener>(1);
        auto dispatcher = std::make_shared<SynchronousEventDispatcher<int>>(listener);
        auto filter = std::make_shared<MockEventFilter>(1, false);
        eventStream.Subscribe({dispatcher, listener, filter, {ECFMP::EventBus::EventDispatchMode::Sync, false}});
        eventStream.OnEvent(1);

        EXPECT_EQ(0, listener->callCount);
        EXPECT_EQ(1, filter->callCount);
    }

    TEST_F(EventStreamTest, OnEventWithOneSubscriptionAndFilterDoesNotCallListenerMultipleTimes)
    {
        auto listener = std::make_shared<MockEventListener>(1);
        auto dispatcher = std::make_shared<SynchronousEventDispatcher<int>>(listener);
        auto filter = std::make_shared<MockEventFilter>(1, false);
        eventStream.Subscribe({dispatcher, listener, filter, {ECFMP::EventBus::EventDispatchMode::Sync, false}});
        eventStream.OnEvent(1);
        eventStream.OnEvent(1);

        EXPECT_EQ(0, listener->callCount);
        EXPECT_EQ(2, filter->callCount);
    }

    TEST_F(EventStreamTest, OnEventWithOneOnceSubscriptionAndFilterCallsListenerOnce)
    {
        auto listener = std::make_shared<MockEventListener>(1);
        auto dispatcher = std::make_shared<SynchronousEventDispatcher<int>>(listener);
        auto filter = std::make_shared<MockEventFilter>(1, true);
        eventStream.Subscribe({dispatcher, listener, filter, {ECFMP::EventBus::EventDispatchMode::Sync, true}});
        eventStream.OnEvent(1);
        eventStream.OnEvent(1);

        EXPECT_EQ(1, listener->callCount);
        EXPECT_EQ(1, filter->callCount);
    }

    TEST_F(EventStreamTest, OnEventWithOneOnceSubscriptionAndFilterDoesNotCallListenerOnce)
    {
        auto listener = std::make_shared<MockEventListener>(1);
        auto dispatcher = std::make_shared<SynchronousEventDispatcher<int>>(listener);
        auto filter = std::make_shared<MockEventFilter>(1, false);
        eventStream.Subscribe({dispatcher, listener, filter, {ECFMP::EventBus::EventDispatchMode::Sync, true}});
        eventStream.OnEvent(1);
        eventStream.OnEvent(1);

        EXPECT_EQ(0, listener->callCount);
        EXPECT_EQ(2, filter->callCount);
    }
}// namespace ECFMP::EventBus
