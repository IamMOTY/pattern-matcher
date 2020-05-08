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

    static bool testOptions(const InstrumentType &);

    bool operator==(Component val) const {
        return this->type == val.type && std::abs(this->ratio - val.ratio) < 1e-7 && this->strike == val.strike &&
               Time::comp(this->expiration, val.expiration) == 0;
    };

    bool operator!=(Component val) const {
        return this->type != val.type || std::abs(this->ratio - val.ratio) > 1e-7 || this->strike != val.strike ||
               Time::comp(this->expiration, val.expiration) != 0;
    };

    InstrumentType type{InstrumentType::Unknown};

    double ratio{0};
    double strike{0};
    std::tm expiration;
};

template<>
struct std::hash<Component>
{
    typedef Component argument_type;
    typedef std::size_t result_type;

    result_type operator()(argument_type const& component) const
    {
        result_type const h1 ( std::hash<double>()(component.ratio));
        result_type const h2 ( std::hash<double>()(component.strike));
        result_type const h3 ( Time::absoluteCountOfDays(component.expiration));
        result_type const h4 (static_cast<size_t>(component.type));

        return h1 ^ (h2 << 1) ^ (h3 << 2) ^ (h4 << 3);
    }
};

