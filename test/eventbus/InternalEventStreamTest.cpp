#include "eventbus/InternalEventStream.h"
#include "ECFMP/NewEventListener.h"

namespace ECFMP::EventBus {

    class MockEventListener : public NewEventListener<int>
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

    class MockEventFilter : public NewEventFilter<int>
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

    class InternalEventStreamTest : public testing::Test
    {
        public:
        InternalEventStream<int> eventStream;
    };

    TEST_F(InternalEventStreamTest, OnEventWithNoSubscriptionsDoesNotThrow)
    {
        ASSERT_NO_THROW(eventStream.OnEvent(1));
    }

    TEST_F(InternalEventStreamTest, ItThrowsOnSubscribeIfListenerIsNull)
    {
        ASSERT_THROW(eventStream.Subscribe(nullptr), std::invalid_argument);
    }

    TEST_F(InternalEventStreamTest, ItThrowsOnSubscribeOnceIfListenerIsNull)
    {
        ASSERT_THROW(eventStream.SubscribeOnce(nullptr), std::invalid_argument);
    }

    TEST_F(InternalEventStreamTest, OnEventWithOneSubscriptionCallsListener)
    {
        auto listener = std::make_shared<MockEventListener>(1);
        eventStream.Subscribe(listener);
        eventStream.OnEvent(1);

        EXPECT_EQ(1, listener->callCount);
    }

    TEST_F(InternalEventStreamTest, OnEventWithOneSubscriptionCallsListenerMultipleTimes)
    {
        auto listener = std::make_shared<MockEventListener>(1);
        eventStream.Subscribe(listener);
        eventStream.OnEvent(1);
        eventStream.OnEvent(1);

        EXPECT_EQ(2, listener->callCount);
    }

    TEST_F(InternalEventStreamTest, OnEventWithOneSubscriptionOnceCallsListenerOnce)
    {
        auto listener = std::make_shared<MockEventListener>(1);
        eventStream.SubscribeOnce(listener);
        eventStream.OnEvent(1);
        eventStream.OnEvent(1);

        EXPECT_EQ(1, listener->callCount);
    }

    TEST_F(InternalEventStreamTest, OnEventWithOneSubscriptionAndFilterCallsListener)
    {
        auto listener = std::make_shared<MockEventListener>(1);
        auto filter = std::make_shared<MockEventFilter>(1, true);
        eventStream.Subscribe(listener, filter);
        eventStream.OnEvent(1);

        EXPECT_EQ(1, listener->callCount);
        EXPECT_EQ(1, filter->callCount);
    }

    TEST_F(InternalEventStreamTest, OnEventWithOneSubscriptionAndFilterCallsListenerMultipleTimes)
    {
        auto listener = std::make_shared<MockEventListener>(1);
        auto filter = std::make_shared<MockEventFilter>(1, true);
        eventStream.Subscribe(listener, filter);
        eventStream.OnEvent(1);
        eventStream.OnEvent(1);

        EXPECT_EQ(2, listener->callCount);
        EXPECT_EQ(2, filter->callCount);
    }

    TEST_F(InternalEventStreamTest, OnEventWithOneSubscriptionAndFilterDoesNotCallListener)
    {
        auto listener = std::make_shared<MockEventListener>(1);
        auto filter = std::make_shared<MockEventFilter>(1, false);
        eventStream.Subscribe(listener, filter);
        eventStream.OnEvent(1);

        EXPECT_EQ(0, listener->callCount);
        EXPECT_EQ(1, filter->callCount);
    }

    TEST_F(InternalEventStreamTest, OnEventWithOneSubscriptionAndFilterDoesNotCallListenerMultipleTimes)
    {
        auto listener = std::make_shared<MockEventListener>(1);
        auto filter = std::make_shared<MockEventFilter>(1, false);
        eventStream.Subscribe(listener, filter);
        eventStream.OnEvent(1);
        eventStream.OnEvent(1);

        EXPECT_EQ(0, listener->callCount);
        EXPECT_EQ(2, filter->callCount);
    }

    TEST_F(InternalEventStreamTest, OnEventWithOneOnceSubscriptionAndFilterCallsListenerOnce)
    {
        auto listener = std::make_shared<MockEventListener>(1);
        auto filter = std::make_shared<MockEventFilter>(1, true);
        eventStream.SubscribeOnce(listener, filter);
        eventStream.OnEvent(1);
        eventStream.OnEvent(1);

        EXPECT_EQ(1, listener->callCount);
        EXPECT_EQ(1, filter->callCount);
    }

    TEST_F(InternalEventStreamTest, OnEventWithOneOnceSubscriptionAndFilterDoesNotCallListenerOnce)
    {
        auto listener = std::make_shared<MockEventListener>(1);
        auto filter = std::make_shared<MockEventFilter>(1, false);
        eventStream.SubscribeOnce(listener, filter);
        eventStream.OnEvent(1);
        eventStream.OnEvent(1);

        EXPECT_EQ(0, listener->callCount);
        EXPECT_EQ(2, filter->callCount);
    }
}// namespace ECFMP::EventBus
