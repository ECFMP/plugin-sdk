#pragma once
#include "ECFMP/Logger.h"
#include <string>

namespace ECFMP::Log {

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

}// namespace ECFMP::Log
