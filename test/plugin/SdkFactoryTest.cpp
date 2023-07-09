#include "flow-sdk/SdkFactory.h"
#include "flow-sdk/EventListeners.h"
#include "mock/MockHttpClient.h"
#include "mock/MockLogger.h"

namespace FlowSdkTest::Plugin {
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
        const auto instance = FlowSdk::Plugin::SdkFactory::Build()
                                      .WithLogger(std::move(logger))
                                      .WithHttpClient(std::move(http))
                                      .Instance();

        instance->Destroy();
    }

    TEST_F(SdkFactoryTest, ItBuildsAnSdkWithNoLogger)
    {
        const auto instance = FlowSdk::Plugin::SdkFactory::Build().WithHttpClient(std::move(http)).Instance();

        instance->Destroy();
    }

    TEST_F(SdkFactoryTest, ItThrowsAnExceptionIfDuplicateHttpClientProvided)
    {
        EXPECT_THROW(
                static_cast<void>(FlowSdk::Plugin::SdkFactory::Build()
                                          .WithHttpClient(std::move(http))
                                          .WithHttpClient(std::move(http2))),
                FlowSdk::Plugin::SdkConfigurationException
        );
    }

    TEST_F(SdkFactoryTest, ItThrowsAnExceptionIfDuplicateLoggerProvided)
    {
        EXPECT_THROW(
                static_cast<void>(FlowSdk::Plugin::SdkFactory::Build()
                                          .WithLogger(std::move(logger))
                                          .WithLogger(std::move(logger2))),
                FlowSdk::Plugin::SdkConfigurationException
        );
    }

    TEST_F(SdkFactoryTest, ItThrowsAnExceptionIfNoHttpClientProvided)
    {
        EXPECT_THROW(
                static_cast<void>(FlowSdk::Plugin::SdkFactory::Build().WithLogger(std::move(logger)).Instance()),
                FlowSdk::Plugin::SdkConfigurationException
        );
    }
}// namespace FlowSdkTest::Plugin
