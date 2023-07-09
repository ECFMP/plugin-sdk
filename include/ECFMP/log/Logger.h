#pragma once

namespace ECFMP::Log {

    /**
     * A class that plugins consuming the SDK may implement in order to allow ECFMP to log to their logs.
     */
    class Logger
    {
        public:
        virtual ~Logger() = default;
        virtual void Debug(const std::string& message) = 0;
        virtual void Info(const std::string& message) = 0;
        virtual void Warning(const std::string& message) = 0;
        virtual void Error(const std::string& message) = 0;
    };
}// namespace ECFMP::Log
