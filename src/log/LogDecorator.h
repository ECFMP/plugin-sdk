#pragma once
#include "flow-sdk/Logger.h"
#include <string>

namespace FlowSdk ::Log {

    /**
     * Wraps the user-provided logger and decorates
     * it with ECFMP so they know the logs come from us and we don't
     * have to tag it on to every log.
     */
    class LogDecorator : public Logger
    {
        public:
        explicit LogDecorator(std::unique_ptr<Log::Logger> userLogger);
        ~LogDecorator();
        void Debug(const std::string& message) override;
        void Info(const std::string& message) override;
        void Warning(const std::string& message) override;
        void Error(const std::string& message) override;

        private:
        std::unique_ptr<Log::Logger> userLogger;
    };

}// namespace FlowSdk::Log
