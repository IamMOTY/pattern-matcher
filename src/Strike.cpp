#include "Strike.h"

bool Strike::isBear() const {
    return this->sym == 0 || isupper(this->sym);
}

bool Strike::isCheckable() const {
    return isupper(this->sym) || this->sym == '+' || this->sym == '-';
}


Strike::Strike() = default;