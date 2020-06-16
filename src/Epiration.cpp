#include "Expiration.h"

bool Expiration::isbear() const {
    return this->sym == 0 || isupper(this->sym);
}

bool Expiration::ischeckable() const {
    return isupper(this->sym) || this->sym == '+';
}

bool Expiration::isdelta() const {
    return islower(this->sym);
}

Expiration::Expiration() = default;

