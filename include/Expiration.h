#include <string>

class Expiration {
public:
    Expiration(const std::string &string) {
        if (string == "") {
            sym = 0;
            count = -1;
        } else if (string[0] == '-') {
            sym = '-';
            count = string.size();
        } else if (string[0] == '+') {
            sym = '+';
            count = string.size();
        } else if (string.back() == 'q' || string.back() == 'm' ||
                   string.back() == 'y' || string.back() == 'd') {
            std::string temp;
            for (char c:string) {
                if (isdigit(c)) {
                    temp += c;
                }
            }
            if (temp.empty()) {
                temp = "1";
            }
            sym = string.back();
            count = std::stoi(temp);
        } else {
            sym = string[0];
            count = 1;
        }

    }

    Expiration() = default;

    bool operator<(Expiration val) const {
        if (isupper(this->sym) || this->sym == 0) {
            return true;
        }
        if (this->sym == val.sym) {
            return this->count < val.count;
        }
        return this->sym < val.sym;
    };

    bool operator==(Expiration val) const {
        return this->sym == val.sym && this->count == val.count;
    };


    [[nodiscard]] bool isbear() const;

    [[nodiscard]] bool ischeckable() const;

    [[nodiscard]] bool isdelta() const;


    int count = 0;
    char sym = 0;

};

template<>
struct std::hash<Expiration> {
    typedef Expiration argument_type;
    typedef std::size_t result_type;

    result_type operator()(argument_type const &expiration) const {
        result_type const h1(std::hash<char>()(expiration.sym));
        result_type const h2(std::hash<int>()(expiration.count));
        return h1 * 13 + h2 * 23;
    }
};
