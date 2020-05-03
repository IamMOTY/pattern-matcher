#pragma once

#include <ctime>
#include <istream>
#include <string>
#include <cmath>
#include "Time.h"

enum class InstrumentType : char {
    C = 'C',
    F = 'F',
    O = 'O',
    P = 'P',
    U = 'U',
    Unknown = '\0'
};

struct Component {
    static Component from_stream(std::istream &);

    static Component from_string(const std::string &);

    static bool testOptions(const InstrumentType&);

    bool operator== (Component val) const {
        return (this->ratio == val.ratio && this->strike == val.strike && Time::comp(this->expiration, val.expiration) == 0);
    };
    bool operator!= (Component val) const {
        return (this->ratio != val.ratio || this->strike != val.strike || Time::comp(this->expiration, val.expiration) != 0);
    };
    bool operator> (Component val) const {
        return this->ratio > val.ratio;
    };
    bool operator< (Component val) const {
        return this->ratio < val.ratio;
    };



    InstrumentType type{InstrumentType::Unknown};
    double ratio{0};
    double strike{0};
    std::tm expiration;
};

