#pragma once

namespace ECFMP::Time {
    [[nodiscard]] auto TimeNow() -> std::chrono::system_clock::time_point;
    void SetTestNow(const std::chrono::system_clock::time_point& now);
    void UnsetTestNow();
}// namespace ECFMP::Time
