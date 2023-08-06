#include "ParseDateStrings.h"
#include "chrono"
#include <ctime>

namespace ECFMP::Date {
    const std::string timeFormat = "%FT%T%Z";

    auto DateStringValid(const std::string& date) -> bool
    {
        std::chrono::system_clock::time_point timePoint;
        std::istringstream inputStream(date);
        inputStream >> std::chrono::parse(timeFormat, timePoint);

        return static_cast<bool>(inputStream);
    }

    auto TimePointFromDateString(const std::string& date) -> std::chrono::system_clock::time_point
    {
        std::chrono::system_clock::time_point timePoint;
        std::istringstream inputStream(date);
        inputStream >> std::chrono::parse(timeFormat, timePoint);

        return static_cast<bool>(inputStream) ? timePoint : (std::chrono::system_clock::time_point::max)();
    }

    auto DateStringFromTimePoint(const std::chrono::system_clock::time_point& timePoint) -> std::string
    {
        std::time_t time = std::chrono::system_clock::to_time_t(timePoint);
        std::tm timeInfo{};
        gmtime_s(&timeInfo, &time);

        std::ostringstream oss;

        // Slightly different format here
        oss << std::put_time(&timeInfo, "%FT%TZ");

        return oss.str();
    }
}// namespace ECFMP::Date
