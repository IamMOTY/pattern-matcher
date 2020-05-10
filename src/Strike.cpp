#include "Strike.h"

bool Strike::isbear() const {
    return this->sym == 0 || isupper(this->sym);
}

bool Strike::ischeckable() const {
    return isupper(this->sym) || this->sym == '+' || this->sym == '-';
}


Strike::Strike() = default;