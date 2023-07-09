#include "NullLogger.h"

namespace ECFMP::Log {

    void NullLogger::Debug(const std::string& message)
    {
        // Noop
    }

    void NullLogger::Info(const std::string& message)
    {
        // Noop
    }

    void NullLogger::Warning(const std::string& message)
    {
        // Noop
    }

    void NullLogger::Error(const std::string& message)
    {
        // Noop
    }
}// namespace ECFMP::Log
