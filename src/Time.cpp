#include "Time.h"


int Time::comp(std::tm first, std::tm second) {
    if (std::difftime(mktime(&first), mktime(&second)) < 0) {
        return -1;
    }
    if (std::difftime(mktime(&first), mktime(&second)) == 0) {
        return 0;
    }
    if (std::difftime(mktime(&first), mktime(&second)) > 0) {
        return 1;
    }
}

int Time::diffInDays(std::tm first, std::tm second) {
    return absoluteCountOfDays(second) - absoluteCountOfDays(first);
}

int Time::countLeapYears(std::tm date) {
    int years = date.tm_year + 1900;
    if (date.tm_mon < 2) {
        years--;
    }
    return years / 4 - years / 100 + years / 400;
}

int Time::absoluteCountOfDays(std::tm date) {
    long long result = (date.tm_year + 1900) * 365 + date.tm_mday;
    for (int i = 0; i < date.tm_mon; i++) {
        result += monthDay[i];
    }
    return result + countLeapYears(date);
}

int Time::diffInMonth(tm first, tm second) {
    if (first.tm_mday != second.tm_mday) {
        return -1;
    }
    if (first.tm_mon > second.tm_mon) {
        return 12 + second.tm_mon - first.tm_mon;
    } else {
        return second.tm_mon - first.tm_mon;
    }
}

int Time::diffInQuart(tm first, tm second) {
    int result;
    if (first.tm_mon > second.tm_mon) {
        result = 12 + second.tm_mon - first.tm_mon;
    } else {
        result = second.tm_mon - first.tm_mon;
    }
    return result / 3;
}

int Time::diffInYears(tm first, tm second) {
    if (first.tm_mday != second.tm_mday || first.tm_mon != second.tm_mon) {
        return -1;
    }
    return second.tm_year - first.tm_year;
}






