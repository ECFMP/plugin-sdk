#include "plugin/ConcreteSdk.h"
#include "ECFMP/eventbus/EventListener.h"
#include "eventbus/InternalEventBusFactory.h"
#include "mock/MockHttpClient.h"

namespace ECFMPTest::Plugin {

    template<typename EventType>
    class MockEventListener : public ECFMP::EventBus::EventListener<EventType>
    {
        public:
        void OnEvent(const EventType&) override
        {
            callCount++;
        }

        int callCount = 0;
    };

    class ConcreteSdkTest : public testing::Test
    {
        public:
        ConcreteSdkTest()
            : eventBus(ECFMP::EventBus::MakeEventBus()), testPtr(std::make_shared<int>(5)),
              instance(std::shared_ptr<void>(testPtr), eventBus)
        {}

        std::shared_ptr<ECFMP::EventBus::InternalEventBus> eventBus;
        std::shared_ptr<int> testPtr;
        ECFMP::Plugin::ConcreteSdk instance;
    };

    TEST_F(ConcreteSdkTest, OnEuroscopeTimerTickTriggersEvent)
    {
        auto mockEventListener = std::make_shared<MockEventListener<ECFMP::Plugin::EuroscopeTimerTickEvent>>();
        eventBus->SubscribeSync<ECFMP::Plugin::EuroscopeTimerTickEvent>(mockEventListener);
        EXPECT_EQ(0, mockEventListener->callCount);
        instance.OnEuroscopeTimerTick();
        EXPECT_EQ(1, mockEventListener->callCount);
    }

    TEST_F(ConcreteSdkTest, ItHasAnEventBus)
    {
        auto mockEventListener = std::make_shared<MockEventListener<int>>();
        instance.EventBus().Subscribe<int>(mockEventListener);
        eventBus->OnEvent<int>(5);
        instance.OnEuroscopeTimerTick();
        EXPECT_EQ(1, mockEventListener->callCount);
    }

    TEST_F(ConcreteSdkTest, ItHasADefaultFlightInformationRegionCollection)
    {
        EXPECT_EQ(0, instance.FlightInformationRegions()->Count());
    }

    TEST_F(ConcreteSdkTest, ItUpdatesFlightInformationRegionsOnEvent)
    {
        auto newFirs = std::make_shared<ECFMP::Api::InternalFlightInformationRegionCollection>();
        auto event = ECFMP::Plugin::FlightInformationRegionsUpdatedEvent{newFirs};
        instance.OnEvent(event);

        EXPECT_EQ(newFirs, instance.FlightInformationRegions());
    }

    TEST_F(ConcreteSdkTest, ItHasADefaultEventCollection)
    {
        EXPECT_EQ(0, instance.Events()->Count());
    }

    TEST_F(ConcreteSdkTest, ItUpdatesEventsOnEvent)
    {
        auto newEvents = std::make_shared<ECFMP::Api::InternalEventCollection>();
        auto event = ECFMP::Plugin::EventsUpdatedEvent{newEvents};
        instance.OnEvent(event);

        EXPECT_EQ(newEvents, instance.Events());
    }

    TEST_F(ConcreteSdkTest, ItHasADefaultFlowMeasureCollection)
    {
        EXPECT_EQ(0, instance.FlowMeasures()->Count());
    }

    TEST_F(ConcreteSdkTest, ItUpdatesFlowMeasuresOnEvent)
    {
        auto newFlowMeasures = std::make_shared<ECFMP::Api::InternalFlowMeasureCollection>();
        auto event = ECFMP::Plugin::FlowMeasuresUpdatedEvent{newFlowMeasures};
        instance.OnEvent(event);

        EXPECT_EQ(newFlowMeasures, instance.FlowMeasures());
    }
}// namespace ECFMPTest::Plugin
