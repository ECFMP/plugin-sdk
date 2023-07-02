#pragma once

namespace FlowSdk::Date {
    [[nodiscard]] auto DateStringValid(const std::string& date) -> bool;
    [[nodiscard]] auto TimePointFromDateString(const std::string& date) -> std::chrono::system_clock::time_point;
    [[nodiscard]] auto DateStringFromTimePoint(const std::chrono::system_clock::time_point& timePoint) -> std::string;
}// namespace FlowSdk::Date
