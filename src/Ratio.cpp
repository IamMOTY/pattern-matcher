#include "Ratio.h"
#include <cmath>

bool Ratio::match(double value) const {
    if (this->c == INF) {
        return value > 0;
    }
    if (this->c == -INF) {
        return value < 0;
    }
    return std::abs(value - c) < 1e-7;
}


