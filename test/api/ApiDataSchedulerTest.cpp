#include "api/ApiDataScheduler.h"
#include "ECFMP/eventbus/EventListener.h"
#include "eventbus/InternalEventBusFactory.h"
#include "time/Clock.h"

namespace ECFMPTest::Api {

    class ApiDataDownloadRequiredEventListener
        : public ECFMP::EventBus::EventListener<ECFMP::Plugin::ApiDataDownloadRequiredEvent>
    {
        public:
        void OnEvent(const ECFMP::Plugin::ApiDataDownloadRequiredEvent& eventType) override
        {
            callCount++;
        }

        int callCount = 0;
    };

    class ApiDataSchedulerTest : public testing::Test
    {
        public:
        ApiDataSchedulerTest()
            : listener(std::make_shared<ApiDataDownloadRequiredEventListener>()),
              eventBus(ECFMP::EventBus::MakeEventBus()),
              apiDataScheduler(std::make_shared<ECFMP::Api::ApiDataScheduler>(eventBus))
        {
            eventBus->SubscribeSync<ECFMP::Plugin::ApiDataDownloadRequiredEvent>(listener);
        }

        void TearDown() override
        {
            ECFMP::Time::UnsetTestNow();
        }

        std::shared_ptr<ApiDataDownloadRequiredEventListener> listener =
                std::make_shared<ApiDataDownloadRequiredEventListener>();
        std::shared_ptr<ECFMP::EventBus::InternalEventBus> eventBus;
        std::shared_ptr<ECFMP::Api::ApiDataScheduler> apiDataScheduler;
    };

    TEST_F(ApiDataSchedulerTest, ItPerformsUpdateOnFirstTick)
    {
        apiDataScheduler->OnEvent({});
        EXPECT_EQ(1, listener->callCount);
    }

    TEST_F(ApiDataSchedulerTest, ItPerformsUpdateIfOneNotPerformedFor90Seconds)
    {
        ECFMP::Time::SetTestNow(std::chrono::system_clock::now());
        apiDataScheduler->OnEvent({});
        ECFMP::Time::SetTestNow(ECFMP::Time::TimeNow() + std::chrono::seconds(91));
        apiDataScheduler->OnEvent({});

        EXPECT_EQ(2, listener->callCount);
    }

    TEST_F(ApiDataSchedulerTest, ItDoesntPerformUpdateIfOnePerformedRecently)
    {
        ECFMP::Time::SetTestNow(std::chrono::system_clock::now());
        apiDataScheduler->OnEvent({});
        ECFMP::Time::SetTestNow(ECFMP::Time::TimeNow() + std::chrono::seconds(89));
        apiDataScheduler->OnEvent({});

        EXPECT_EQ(1, listener->callCount);
    }
}// namespace ECFMPTest::Api
