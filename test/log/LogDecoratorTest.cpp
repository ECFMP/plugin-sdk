#include "log/LogDecorator.h"
#include "mock/MockLogger.h"

namespace FlowSdkTest::Log {
    class LogDecoratorTest : public testing::Test
    {
        public:
        LogDecoratorTest() : logger(std::make_unique<testing::NiceMock<MockLogger>>())
        {}

        std::unique_ptr<testing::NiceMock<MockLogger>> logger;
    };

    TEST_F(LogDecoratorTest, ItDecoratesDebugLogs)
    {
        EXPECT_CALL(*logger.get(), Debug("ECFMP: Hi")).Times(1);

        FlowSdk::Log::LogDecorator decorator(std::move(logger));
        decorator.Debug("Hi");
    }

    TEST_F(LogDecoratorTest, ItDecoratesInfoLogs)
    {
        EXPECT_CALL(*logger.get(), Info("ECFMP: Hi")).Times(1);

        FlowSdk::Log::LogDecorator decorator(std::move(logger));
        decorator.Info("Hi");
    }

    TEST_F(LogDecoratorTest, ItDecoratesWarningLogs)
    {
        EXPECT_CALL(*logger.get(), Warning("ECFMP: Hi")).Times(1);

        FlowSdk::Log::LogDecorator decorator(std::move(logger));
        decorator.Warning("Hi");
    }

    TEST_F(LogDecoratorTest, ItDecoratesErrorLogs)
    {
        EXPECT_CALL(*logger.get(), Error("ECFMP: Hi")).Times(1);

        FlowSdk::Log::LogDecorator decorator(std::move(logger));
        decorator.Error("Hi");
    }
}// namespace FlowSdkTest::Log
