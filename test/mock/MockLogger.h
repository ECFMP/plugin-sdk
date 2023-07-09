#pragma once
#include "ECFMP/Logger.h"

namespace ECFMPTest::Log {
    class MockLogger : public ECFMP::Log::Logger
    {
        public:
        MOCK_METHOD(void, Debug, (const std::string&), (override));
        MOCK_METHOD(void, Info, (const std::string&), (override));
        MOCK_METHOD(void, Warning, (const std::string&), (override));
        MOCK_METHOD(void, Error, (const std::string&), (override));
    };
}// namespace ECFMPTest::Log
