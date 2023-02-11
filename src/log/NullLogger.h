#pragma once
#include "flow-sdk/Logger.h"
#include <string>

namespace FlowSdk::Log {

    /**
         * A logger that logs nothing, a default if the user doesn't
         * provide one.
         */
    class NullLogger : public Logger
    {
        public:
        void Debug(const std::string& message) override;
        void Info(const std::string& message) override;
        void Warning(const std::string& message) override;
        void Error(const std::string& message) override;
    };

}// namespace FlowSdk::Log
