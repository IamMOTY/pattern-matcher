#include <string>

class Strike {
public:
    Strike(const std::string &string) {
        if (string == "") {
            sym = 0;
            count = -1;
        } else if (string[0] == '-') {
            sym = '-';
            count = string.size();
        } else if (string[0] == '+') {
            sym = '+';
            count = string.size();
        } else {
            sym = string[0];
            count = 1;
        }
    }

    Strike();

    bool operator<(Strike val) const {
        if (isupper(this->sym) || this->sym == 0) {
            return true;
        }
        if (this->sym == val.sym) {
            return this->count < val.count;
        }
        return this->sym < val.sym;
    };

    bool operator==(Strike val) const {
        return this->sym == val.sym && this->count == val.count;
    };


    [[nodiscard]] bool isbear() const;

    [[nodiscard]] bool ischeckable() const;

    [[nodiscard]] bool isdelta() const;


    int count = 0;
    char sym = 0;

};

template<>
struct std::hash<Strike> {
    typedef Strike argument_type;
    typedef std::size_t result_type;

    result_type operator()(argument_type const &strike) const {
        result_type const h1(std::hash<char>()(strike.sym));
        result_type const h2(std::hash<int>()(strike.count));
        return h1 * 13 + h2 * 23;
    }
};
