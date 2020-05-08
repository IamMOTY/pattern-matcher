#pragma once

#include <ctime>


class Time {
public:

    static int absoluteCountOfDays(const tm &date);

    static int diffInDays(const tm &first, const tm &second);

    static int diffInMonth(const tm &first, const tm &second);

    static int diffInQuart(const tm &first, const tm &second);

    static int diffInYears(const tm &first, const tm &second);

    static int countLeapYears(const tm &date);

    static int comp(const tm &first, const tm &second);

    constexpr static const int monthDay[12] = {31, 28, 31, 30, 31, 30,
                                               31, 31, 30, 31, 30, 31};
private:
};