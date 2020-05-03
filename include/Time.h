#pragma once

#include <ctime>


class Time {
public:
    static int diffInDays(tm first, tm second);

    static int diffInMonth(tm first, tm second);

    static int diffInQuart(tm first, tm second);

    static int diffInYears(tm first, tm second);

    static int comp(std::tm, std::tm);

    constexpr static const int monthDay[12] = {31, 28, 31, 30, 31, 30,
                                               31, 31, 30, 31, 30, 31};

private:
    static int countLeapYears(std::tm date);

    static int absoluteCountOfDays(std::tm date);


};
