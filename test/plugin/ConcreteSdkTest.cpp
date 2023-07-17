#include "plugin/ConcreteSdk.h"
#include "ECFMP/eventbus/EventListener.h"
#include "api/ApiDataDownloader.h"
#include "eventbus/InternalEventBus.h"
#include "mock/MockHttpClient.h"

namespace ECFMPTest::Plugin {

    class MockEventListener : public ECFMP::EventBus::EventListener<int>
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
            : eventBus(std::make_shared<ECFMP::EventBus::InternalEventBus>()), testPtr(std::make_shared<int>(5)),
              instance(std::shared_ptr<void>(testPtr), eventBus)
        {}

        std::shared_ptr<ECFMP::EventBus::InternalEventBus> eventBus;
        std::shared_ptr<int> testPtr;
        ECFMP::Plugin::ConcreteSdk instance;
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
}// namespace ECFMPTest::Plugin
