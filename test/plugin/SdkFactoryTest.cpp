#include "ECFMP/SdkFactory.h"
#include "ECFMP/SdkEvents.h"
#include "ECFMP/eventbus/EventBus.h"
#include "api/ApiDataDownloadedEvent.h"
#include "api/ApiDataParser.h"
#include "mock/MockHttpClient.h"
#include "mock/MockLogger.h"
#include "plugin/ConcreteSdk.h"
#include "plugin/InternalSdkEvents.h"

namespace ECFMPTest::Plugin {
    class SdkFactoryTest : public testing::Test
    {
        public:
        SdkFactoryTest()
            : logger(std::make_unique<testing::NiceMock<Log::MockLogger>>()),
              logger2(std::make_unique<testing::NiceMock<Log::MockLogger>>()),
              http(std::make_unique<testing::NiceMock<Http::MockHttpClient>>()),
              http2(std::make_unique<testing::NiceMock<Http::MockHttpClient>>())
        {}

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
                instance->EventBus().HasListenerOfType<ECFMP::Api::ApiDataParser, ECFMP::Api::ApiDataDownloadedEvent>();
        EXPECT_TRUE(hasListener);
        instance->Destroy();
    }

    TEST_F(SdkFactoryTest, ItRegistersSdkForFirUpdateEvents)
    {
        const auto instance = ECFMP::Plugin::SdkFactory::Build().WithHttpClient(std::move(http)).Instance();
        auto hasListener =
                instance->EventBus()
                        .HasListenerOfType<
                                ECFMP::Plugin::ConcreteSdk, ECFMP::Plugin::FlightInformationRegionsUpdatedEvent>();
        EXPECT_TRUE(hasListener);
        instance->Destroy();
    }

    TEST_F(SdkFactoryTest, ItRegistersSdkForEventsUpdateEvents)
    {
        const auto instance = ECFMP::Plugin::SdkFactory::Build().WithHttpClient(std::move(http)).Instance();
        auto hasListener =
                instance->EventBus().HasListenerOfType<ECFMP::Plugin::ConcreteSdk, ECFMP::Plugin::EventsUpdatedEvent>();
        EXPECT_TRUE(hasListener);
        instance->Destroy();
    }

    TEST_F(SdkFactoryTest, ItRegistersSdkForFlowMeasureUpdateEvents)
    {
        const auto instance = ECFMP::Plugin::SdkFactory::Build().WithHttpClient(std::move(http)).Instance();
        auto hasListener =
                instance->EventBus()
                        .HasListenerOfType<ECFMP::Plugin::ConcreteSdk, ECFMP::Plugin::FlowMeasuresUpdatedEvent>();
        EXPECT_TRUE(hasListener);
        instance->Destroy();
    }
}// namespace ECFMPTest::Plugin
