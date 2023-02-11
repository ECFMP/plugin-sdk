#include "plugin/ConcreteEventListeners.h"
#include "mock/FlowMeasureMock.h"
#include "mock/MockEventListener.h"
#include "mock/MockEventListenerFilter.h"

namespace FlowSdkTest::Plugin {

    class ConcreteEventListenersTest : public testing::Test
    {
        public:
        ConcreteEventListenersTest()
            : mockListener(std::make_shared<testing::NiceMock<MockEventListener<FlowSdk::FlowMeasure::FlowMeasure>>>()),
              mockFilter(
                      std::make_shared<testing::NiceMock<MockEventListenerFilter<FlowSdk::FlowMeasure::FlowMeasure>>>())
        {}

        testing::NiceMock<FlowSdk::Mock::FlowMeasure::FlowMeasureMock> measureMock;
        std::shared_ptr<testing::NiceMock<MockEventListener<FlowSdk::FlowMeasure::FlowMeasure>>> mockListener;
        std::shared_ptr<testing::NiceMock<MockEventListenerFilter<FlowSdk::FlowMeasure::FlowMeasure>>> mockFilter;
        FlowSdk::Plugin::ConcreteEventListeners<FlowSdk::FlowMeasure::FlowMeasure> listeners;
    };

    TEST_F(ConcreteEventListenersTest, ItAddsListeners)
    {
        EXPECT_CALL(*mockListener, OnEvent(testing::Ref(measureMock))).Times(1);

        listeners.Add(mockListener);
        listeners.OnEvent(measureMock);
    }

    TEST_F(ConcreteEventListenersTest, ItDoesntAddDuplicateListeners)
    {
        EXPECT_CALL(*mockListener, OnEvent(testing::Ref(measureMock))).Times(1);

        listeners.Add(mockListener);
        listeners.Add(mockListener);
        listeners.Add(mockListener);
        listeners.Add(mockListener);
        listeners.OnEvent(measureMock);
    }

    TEST_F(ConcreteEventListenersTest, ItAddsListenersWithAPassingFilter)
    {
        EXPECT_CALL(*mockFilter, Passes(testing::Ref(measureMock))).Times(1).WillOnce(testing::Return(true));
        EXPECT_CALL(*mockListener, OnEvent(testing::Ref(measureMock))).Times(1);

        listeners.Add(mockListener, mockFilter);
        listeners.OnEvent(measureMock);
    }

    TEST_F(ConcreteEventListenersTest, ItDoesntAddDuplicateListenerWithPassingFilter)
    {
        EXPECT_CALL(*mockFilter, Passes(testing::Ref(measureMock))).Times(1).WillOnce(testing::Return(true));
        EXPECT_CALL(*mockListener, OnEvent(testing::Ref(measureMock))).Times(1);

        listeners.Add(mockListener, mockFilter);
        listeners.Add(mockListener, mockFilter);
        listeners.Add(mockListener, mockFilter);
        listeners.Add(mockListener, mockFilter);
        listeners.Add(mockListener, mockFilter);
        listeners.OnEvent(measureMock);
    }

    TEST_F(ConcreteEventListenersTest, ItAddsListenersWithAFailingFilter)
    {
        EXPECT_CALL(*mockFilter, Passes(testing::Ref(measureMock))).Times(1).WillOnce(testing::Return(false));
        EXPECT_CALL(*mockListener, OnEvent(testing::Ref(measureMock))).Times(0);

        listeners.Add(mockListener, mockFilter);
        listeners.OnEvent(measureMock);
    }

    TEST_F(ConcreteEventListenersTest, ItRemovesAListener)
    {
        EXPECT_CALL(*mockListener, OnEvent(testing::Ref(measureMock))).Times(0);

        listeners.Add(mockListener);
        listeners.Remove(mockListener);
    }

    TEST_F(ConcreteEventListenersTest, ItRemovesAListenerAndFilter)
    {
        EXPECT_CALL(*mockFilter, Passes(testing::Ref(measureMock))).Times(0);
        EXPECT_CALL(*mockListener, OnEvent(testing::Ref(measureMock))).Times(0);

        listeners.Add(mockListener, mockFilter);
        listeners.Remove(mockListener);
        listeners.OnEvent(measureMock);
    }
}// namespace FlowSdkTest::Plugin
