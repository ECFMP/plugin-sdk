#include "LogDecorator.h"

namespace FlowSdk ::Log {

    /**
     * Non-class method for message decoration.
     */
    [[nodiscard]] auto DecorateMessage(const std::string& message) -> std::string
    {
        return "ECFMP: " + message;
    }

    LogDecorator::LogDecorator(std::unique_ptr<Logger> userLogger) : userLogger(std::move(userLogger))
    {
        assert(this->userLogger && "No logger provided to LogDecorator");
    }

    LogDecorator::~LogDecorator() = default;

    void LogDecorator::Debug(const std::string& message)
    {
        userLogger->Debug(DecorateMessage(message));
    }

    void LogDecorator::Info(const std::string& message)
    {
        userLogger->Info(DecorateMessage(message));
    }

    void LogDecorator::Warning(const std::string& message)
    {
        userLogger->Warning(DecorateMessage(message));
    }

    void LogDecorator::Error(const std::string& message)
    {
        userLogger->Error(DecorateMessage(message));
    }
}// namespace FlowSdk::Log
