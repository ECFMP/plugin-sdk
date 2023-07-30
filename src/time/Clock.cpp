#include "Clock.h"

namespace ECFMP::Time {

    std::chrono::system_clock::time_point testNow = std::chrono::system_clock::time_point::min();

    auto TimeNow() -> std::chrono::system_clock::time_point
    {
        if (testNow != std::chrono::system_clock::time_point::min()) {
            return testNow;
        }

        return std::chrono::system_clock::now();
    }

    void SetTestNow(const std::chrono::system_clock::time_point& now)
    {
        testNow = now;
    }

    void UnsetTestNow()
    {
        testNow = std::chrono::system_clock::time_point::min();
    }
}// namespace ECFMP::Time
