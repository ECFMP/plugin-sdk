#include "Clock.h"

namespace ECFMP::Time {

    auto DefaultTime() -> std::chrono::system_clock::time_point
    {
        return (std::chrono::system_clock::time_point::min)();
    }

    std::chrono::system_clock::time_point testNow = DefaultTime();

    auto TimeNow() -> std::chrono::system_clock::time_point
    {
        return testNow != DefaultTime() ? testNow : std::chrono::system_clock::now();
    }

    void SetTestNow(const std::chrono::system_clock::time_point& now)
    {
        testNow = now;
    }

    void UnsetTestNow()
    {
        testNow = DefaultTime();
    }
}// namespace ECFMP::Time
