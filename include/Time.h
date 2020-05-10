#pragma once

#include <ctime>


class Time {
public:

    static int absoluteCountOfDays(const std::tm &date);

    static int diffInDays(const std::tm &first, const std::tm &second);

    static int diffInMonth(const std::tm &first, const std::tm &second);

    static int diffInQuart(const std::tm &first, const std::tm &second);

    static int diffInYears(const std::tm &first, const std::tm &second);

    static int countLeapYears(const std::tm &date);

    static int comp(const std::tm &first, const std::tm &second);

    constexpr static const int monthDay[12] = {31, 28, 31, 30, 31, 30,
                                               31, 31, 30, 31, 30, 31};
private:
};