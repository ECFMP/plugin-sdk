#include "date/ParseDateStrings.h"
#include <chrono>

namespace FlowSdkTest::Date {

    using TimeStringTestParam = struct TimeStringTestParam {
        // The input string
        std::string input;

        // Test case name
        std::string name;

        // The expected time point
        int year;

        int month;

        int day;

        int hours;

        int minutes;

        int seconds;
    };

    using TimeStringValidityTestParam = struct TimeStringValidityTestParam {
        // The input string
        std::string input;

        // Test case name
        std::string name;

        // Expected output
        bool expected;
    };

    using ToTimeStringTestParam = struct ToTimeStringTestParam {
        // Test case name
        std::string name;

        // The input
        int year;

        int month;

        int day;

        int hours;

        int minutes;

        int seconds;

        // The expected output
        std::string expected;
    };

    template<typename T>
    auto TestParamToTimePoint(const typename testing::WithParamInterface<T>::ParamType& testParam)
            -> std::chrono::system_clock::time_point
    {
        tm timeInfo;
        timeInfo.tm_year = testParam.year - 1900;// Years since 1900
        timeInfo.tm_mon = testParam.month - 1;   // Months since January (0-11)
        timeInfo.tm_mday = testParam.day;
        timeInfo.tm_hour = testParam.hours;
        timeInfo.tm_min = testParam.minutes;
        timeInfo.tm_sec = testParam.seconds;
        timeInfo.tm_isdst = 0;

        return std::chrono::system_clock::from_time_t(mktime(&timeInfo));
    }

    class ValidateValidTimeStringsTest : public testing::TestWithParam<TimeStringValidityTestParam>
    {
    };

    TEST_P(ValidateValidTimeStringsTest, ItChecksThatTimeStringsAreValid)
    {
        EXPECT_EQ(GetParam().expected, FlowSdk::Date::DateStringValid(GetParam().input));
    }

    INSTANTIATE_TEST_SUITE_P(
            ValidateValidTimeStringsTestCases, ValidateValidTimeStringsTest,
            testing::Values(
                    TimeStringValidityTestParam{"2023-03-01T20:24:00Z", "standard_time", true},
                    TimeStringValidityTestParam{"2023-10-11T20:24:00Z", "double_digit_date", true},
                    TimeStringValidityTestParam{"2023-03-01T01:26:23Z", "with_seconds", true},
                    TimeStringValidityTestParam{"2023-03-01T01:26:23.000Z", "with_3_fractions", true},
                    TimeStringValidityTestParam{"2023-03-01T01:26:23.000000Z", "with_6_fractions", true},
                    TimeStringValidityTestParam{"2023-03-0101:26:23Z", "no_t", false},
                    TimeStringValidityTestParam{"2023-03-01 01:26:23Z", "space_instead_of_t", false},
                    TimeStringValidityTestParam{"2023-03-01T01:26:23", "no_z", false},
                    TimeStringValidityTestParam{"2023-03-01T01:26", "no_seconds", false}
            ),
            [](const ::testing::TestParamInfo<ValidateValidTimeStringsTest::ParamType>& info) {
                return info.param.name;
            }
    );

    class ParseTimeStringsTest : public testing::TestWithParam<TimeStringTestParam>
    {
    };

    TEST_P(ParseTimeStringsTest, ItParsesTimeStrings)
    {
        const auto expected = TestParamToTimePoint<TimeStringTestParam>(GetParam());
        EXPECT_EQ(expected, FlowSdk::Date::TimePointFromDateString(GetParam().input));
    }

    INSTANTIATE_TEST_SUITE_P(
            ParseTimeStringsTestCases, ParseTimeStringsTest,
            testing::Values(
                    TimeStringTestParam{"2023-03-01T20:24:00Z", "standard_time", 2023, 3, 1, 20, 24, 0},
                    TimeStringTestParam{"2023-10-11T20:24:00Z", "double_digit_date", 2023, 10, 11, 20, 24, 0},
                    TimeStringTestParam{"2023-03-01T01:26:23Z", "with_seconds", 2023, 3, 1, 1, 26, 23},
                    TimeStringTestParam{"2023-03-01T01:26:23.000Z", "with_3_fractions", 2023, 3, 1, 1, 26, 23},
                    TimeStringTestParam{"2023-03-01T01:26:23.000000Z", "with_6_fractions", 2023, 3, 1, 1, 26, 23}
            ),
            [](const ::testing::TestParamInfo<ParseTimeStringsTest::ParamType>& info) {
                return info.param.name;
            }
    );

    class DateStringFromTimePointTest : public testing::TestWithParam<ToTimeStringTestParam>
    {
    };

    TEST_P(DateStringFromTimePointTest, ItConvertsTimePointsToDateStrings)
    {
        const auto timePoint = TestParamToTimePoint<ToTimeStringTestParam>(GetParam());
        EXPECT_EQ(GetParam().expected, FlowSdk::Date::DateStringFromTimePoint(timePoint));
    }

    INSTANTIATE_TEST_SUITE_P(
            DateStringFromTimePointTestCases, DateStringFromTimePointTest,
            testing::Values(

                    ToTimeStringTestParam{"standard_time", 2023, 3, 1, 20, 24, 0, "2023-03-01T20:24:00Z"},
                    ToTimeStringTestParam{"double_digit_date", 2023, 10, 11, 20, 24, 0, "2023-10-11T20:24:00Z"},
                    ToTimeStringTestParam{"with_seconds", 2023, 3, 1, 1, 26, 23, "2023-03-01T01:26:23Z"},
                    ToTimeStringTestParam{"midnight", 2023, 3, 1, 0, 0, 0, "2023-03-01T00:00:00Z"},
                    ToTimeStringTestParam{"1_second_to_midnight", 2023, 3, 1, 23, 59, 59, "2023-03-01T23:59:59Z"}
            ),
            [](const ::testing::TestParamInfo<DateStringFromTimePointTest::ParamType>& info) {
                return info.param.name;
            }
    );
}// namespace FlowSdkTest::Date
