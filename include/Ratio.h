#pragma once

#include <string>

class Ratio {
public:
    Ratio(const std::string &string) {
        if (string == "-") {
            c = -INF;
        } else if (string == "+") {
            c = INF;
        } else {
            c = std::stod(string);
        }
    }

    Ratio() = default;

    [[nodiscard]] bool match(const double &) const ;

private:
    double c = 0;
    double INF = 1 << 16;
};

