#include "Expiration.h"

bool Expiration::isBear() const {
    return this->sym == 0 || isupper(this->sym);
}

bool Expiration::isCheckable() const {
    return isupper(this->sym) || this->sym == '+';
}

bool Expiration::isDelta() const {
    return islower(this->sym);
}

Expiration::Expiration() = default;

