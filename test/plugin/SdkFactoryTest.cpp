#include "ECFMP/SdkFactory.h"
#include "ECFMP/SdkEvents.h"
#include "ECFMP/eventbus/EventBus.h"
#include "ECFMP/flowmeasure/CustomFlowMeasureFilter.h"
#include "api/ApiDataDownloadedEvent.h"
#include "api/ApiDataDownloader.h"
#include "api/ApiDataParser.h"
#include "api/ApiDataScheduler.h"
#include "eventbus/InternalEventBus.h"
#include "eventbus/PendingEuroscopeEvents.h"
#include "eventbus/SubscriptionFlags.h"
#include "flowmeasure/FlowMeasureStatusUpdates.h"
#include "mock/MockHttpClient.h"
#include "mock/MockLogger.h"
#include "plugin/InternalSdk.h"
#include "plugin/InternalSdkEvents.h"

namespace ECFMPTest::Plugin {

    class MockCustomFlowMeasureFilter : public ECFMP::FlowMeasure::CustomFlowMeasureFilter
    {
        public:
        MOCK_METHOD(
                bool, ApplicableToAircraft,
                (const EuroScopePlugIn::CFlightPlan&, const EuroScopePlugIn::CRadarTarget&,
                 const ECFMP::FlowMeasure::FlowMeasure&),
                (const, noexcept, override)
        );
    };

    class SdkFactoryTest : public testing::Test
    {
        public:
        SdkFactoryTest()
            : logger(std::make_unique<testing::NiceMock<Log::MockLogger>>()),
              logger2(std::make_unique<testing::NiceMock<Log::MockLogger>>()),
              http(std::make_unique<testing::NiceMock<Http::MockHttpClient>>()),
              http2(std::make_unique<testing::NiceMock<Http::MockHttpClient>>())
        {}

        [[nodiscard]] static auto InternalBus(const std::shared_ptr<ECFMP::Plugin::Sdk> sdk)
                -> ECFMP::EventBus::InternalEventBus&
        {
            return const_cast<ECFMP::EventBus::InternalEventBus&>(
                    static_cast<const ECFMP::EventBus::InternalEventBus&>(sdk->EventBus())
            );
        }

        std::unique_ptr<testing::NiceMock<Log::MockLogger>> logger;
        std::unique_ptr<testing::NiceMock<Log::MockLogger>> logger2;
        std::unique_ptr<testing::NiceMock<Http::MockHttpClient>> http;
        std::unique_ptr<testing::NiceMock<Http::MockHttpClient>> http2;
    };

    TEST_F(SdkFactoryTest, ItBuildsAnSdk)
    {
        const auto instance = ECFMP::Plugin::SdkFactory::Build()
                                      .WithLogger(std::move(logger))
                                      .WithHttpClient(std::move(http))
                                      .Instance();

        instance->Destroy();
    }

    TEST_F(SdkFactoryTest, ItBuildsAnSdkWithNoLogger)
    {
        const auto instance = ECFMP::Plugin::SdkFactory::Build().WithHttpClient(std::move(http)).Instance();

        instance->Destroy();
    }

    TEST_F(SdkFactoryTest, ItThrowsAnExceptionIfDuplicateHttpClientProvided)
    {
        EXPECT_THROW(
                static_cast<void>(ECFMP::Plugin::SdkFactory::Build()
                                          .WithHttpClient(std::move(http))
                                          .WithHttpClient(std::move(http2))),
                ECFMP::Plugin::SdkConfigurationException
        );
    }

    TEST_F(SdkFactoryTest, ItThrowsAnExceptionIfDuplicateLoggerProvided)
    {
        EXPECT_THROW(
                static_cast<void>(
                        ECFMP::Plugin::SdkFactory::Build().WithLogger(std::move(logger)).WithLogger(std::move(logger2))
                ),
                ECFMP::Plugin::SdkConfigurationException
        );
    }

    TEST_F(SdkFactoryTest, ItThrowsAnExceptionIfNoHttpClientProvided)
    {
        EXPECT_THROW(
                static_cast<void>(ECFMP::Plugin::SdkFactory::Build().WithLogger(std::move(logger)).Instance()),
                ECFMP::Plugin::SdkConfigurationException
        );
    }

    TEST_F(SdkFactoryTest, ItRegistersApiDataParserForApiDataEvents)
    {
        const auto instance = ECFMP::Plugin::SdkFactory::Build().WithHttpClient(std::move(http)).Instance();
        auto hasListener =
                InternalBus(instance)
                        .HasListenerForSubscription<ECFMP::Api::ApiDataParser, ECFMP::Api::ApiDataDownloadedEvent>(
                                {ECFMP::EventBus::EventDispatchMode::Async, false}
                        );
        EXPECT_TRUE(hasListener);
        instance->Destroy();
    }

    TEST_F(SdkFactoryTest, ItRegistersSdkForFirUpdateEvents)
    {
        const auto instance = ECFMP::Plugin::SdkFactory::Build().WithHttpClient(std::move(http)).Instance();
        auto hasListener =
                InternalBus(instance)
                        .HasListenerForSubscription<
                                ECFMP::Plugin::InternalSdk, ECFMP::Plugin::FlightInformationRegionsUpdatedEvent>(
                                {ECFMP::EventBus::EventDispatchMode::Async, false}
                        );
        EXPECT_TRUE(hasListener);
        instance->Destroy();
    }

    TEST_F(SdkFactoryTest, ItRegistersSdkForEventsUpdateEvents)
    {
        const auto instance = ECFMP::Plugin::SdkFactory::Build().WithHttpClient(std::move(http)).Instance();
        auto hasListener =
                InternalBus(instance)
                        .HasListenerForSubscription<ECFMP::Plugin::InternalSdk, ECFMP::Plugin::EventsUpdatedEvent>(
                                {ECFMP::EventBus::EventDispatchMode::Async, false}
                        );
        EXPECT_TRUE(hasListener);
        instance->Destroy();
    }

    TEST_F(SdkFactoryTest, ItRegistersSdkForFlowMeasureUpdateEvents)
    {
        const auto instance = ECFMP::Plugin::SdkFactory::Build().WithHttpClient(std::move(http)).Instance();
        auto hasListener = InternalBus(instance)
                                   .HasListenerForSubscription<
                                           ECFMP::Plugin::InternalSdk, ECFMP::Plugin::FlowMeasuresUpdatedEvent>(
                                           {ECFMP::EventBus::EventDispatchMode::Async, false}
                                   );
        EXPECT_TRUE(hasListener);
        instance->Destroy();
    }

    TEST_F(SdkFactoryTest, ItRegistersPendingEuroscopeEventsForTimerTickEvents)
    {
        const auto instance = ECFMP::Plugin::SdkFactory::Build().WithHttpClient(std::move(http)).Instance();
        auto hasListener =
                InternalBus(instance)
                        .HasListenerForSubscription<
                                ECFMP::EventBus::PendingEuroscopeEvents, ECFMP::Plugin::EuroscopeTimerTickEvent>(
                                {ECFMP::EventBus::EventDispatchMode::Sync, false}
                        );
        EXPECT_TRUE(hasListener);
        instance->Destroy();
    }

    TEST_F(SdkFactoryTest, ItRegistersApiDataSchedulerForTimerTickEvents)
    {
        const auto instance = ECFMP::Plugin::SdkFactory::Build().WithHttpClient(std::move(http)).Instance();
        auto hasListener = InternalBus(instance)
                                   .HasListenerForSubscription<
                                           ECFMP::Api::ApiDataScheduler, ECFMP::Plugin::EuroscopeTimerTickEvent>(
                                           {ECFMP::EventBus::EventDispatchMode::Sync, false}
                                   );
        EXPECT_TRUE(hasListener);
        instance->Destroy();
    }

    TEST_F(SdkFactoryTest, ItRegistersApiDataDownloaderForDownloadRequiredEvents)
    {
        const auto instance = ECFMP::Plugin::SdkFactory::Build().WithHttpClient(std::move(http)).Instance();
        auto hasListener = InternalBus(instance)
                                   .HasListenerForSubscription<
                                           ECFMP::Api::ApiDataDownloader, ECFMP::Plugin::ApiDataDownloadRequiredEvent>(
                                           {ECFMP::EventBus::EventDispatchMode::Async, false}
                                   );
        EXPECT_TRUE(hasListener);
        instance->Destroy();
    }

    TEST_F(SdkFactoryTest, ItRegistersFlowMeasureStatusUpdatesForUpdatedEvent)
    {
        const auto instance = ECFMP::Plugin::SdkFactory::Build().WithHttpClient(std::move(http)).Instance();
        auto hasListener = InternalBus(instance)
                                   .HasListenerForSubscription<
                                           ECFMP::FlowMeasure::FlowMeasureStatusUpdates,
                                           ECFMP::Plugin::InternalFlowMeasuresUpdatedEvent>(
                                           {ECFMP::EventBus::EventDispatchMode::Async, false}
                                   );
        EXPECT_TRUE(hasListener);
        instance->Destroy();
    }

    TEST_F(SdkFactoryTest, ItAddsCustomFilters)
    {
        const auto customFilter = std::make_shared<MockCustomFlowMeasureFilter>();
        const auto customFilter2 = std::make_shared<MockCustomFlowMeasureFilter>();
        const auto instance = ECFMP::Plugin::SdkFactory::Build()
                                      .WithHttpClient(std::move(http))
                                      .WithCustomFlowMeasureFilter(customFilter)
                                      .WithCustomFlowMeasureFilter(customFilter2)
                                      .Instance();

        const auto customerFilters = ((ECFMP::Plugin::InternalSdk*) instance.get())->CustomFlowMeasureFilters();
        EXPECT_EQ(2, customerFilters.size());
        EXPECT_EQ(customFilter, customerFilters[0]);
        EXPECT_EQ(customFilter2, customerFilters[1]);
        instance->Destroy();
    }
}// namespace ECFMPTest::Plugin
