#pragma once
#include "flow-sdk/Logger.h"

namespace FlowSdkTest::Log {
    class MockLogger : public FlowSdk::Log::Logger
    {
        public:
        MOCK_METHOD(void, Debug, (const std::string&), (override));
        MOCK_METHOD(void, Info, (const std::string&), (override));
        MOCK_METHOD(void, Warning, (const std::string&), (override));
        MOCK_METHOD(void, Error, (const std::string&), (override));
    };
}// namespace FlowSdkTest::Log
