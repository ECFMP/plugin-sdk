#include "date/ParseDateStrings.h"

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
        tm timeInfo;
        timeInfo.tm_year = GetParam().year - 1900;// Years since 1900
        timeInfo.tm_mon = GetParam().month - 1;   // Months since January (0-11)
        timeInfo.tm_mday = GetParam().day;
        timeInfo.tm_hour = GetParam().hours;
        timeInfo.tm_min = GetParam().minutes;
        timeInfo.tm_sec = GetParam().seconds;
        timeInfo.tm_isdst = 0;

        EXPECT_EQ(
                std::chrono::system_clock::from_time_t(mktime(&timeInfo)),
                FlowSdk::Date::TimePointFromDateString(GetParam().input)
        );
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
}// namespace FlowSdkTest::Date
