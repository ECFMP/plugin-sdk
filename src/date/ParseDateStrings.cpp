#include "ParseDateStrings.h"

namespace FlowSdk::Date {
    auto DateStringValid(const std::string& date) -> bool
    {
        std::chrono::system_clock::time_point timePoint;
        std::istringstream inputStream(date);
        inputStream >> std::chrono::parse(std::string{"%FT%T%Z"}, timePoint);

        return static_cast<bool>(inputStream);
    }

    auto TimePointFromDateString(const std::string& date) -> std::chrono::system_clock::time_point
    {
        std::chrono::system_clock::time_point timePoint;
        std::istringstream inputStream(date);
        inputStream >> std::chrono::parse(std::string{"%FT%T%Z"}, timePoint);

        return static_cast<bool>(inputStream) ? timePoint : std::chrono::system_clock::time_point::max();
    }
}// namespace FlowSdk::Date
