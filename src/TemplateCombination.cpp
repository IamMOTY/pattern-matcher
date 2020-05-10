#include <utility>
#include <set>
#include <algorithm>
#include <bits/unordered_set.h>
#include "Time.h"
#include "TemplateCombination.h"


namespace {
    bool check_same_values_expr(const TemplateCombination &templateCombination, const std::vector<Component> &fixed,
                                const std::vector<int> &order) {
        bool flag = true;
        std::tm def{};
        std::unordered_map<Expiration, std::tm> check;
        for (int k = 0; k < templateCombination.count && flag; k++) {
            Expiration temp_exp = templateCombination.legs[k].expiration;
            if (temp_exp.ischeckable()) {
                if (Time::comp(check[temp_exp], def) == 0) {
                    check[temp_exp] = fixed[order[k]].expiration;
                } else {
                    if (Time::comp(check[temp_exp], fixed[order[k]].expiration) != 0) {
                        flag = false;
                        continue;
                    }
                }
            }
        }
        return flag;
    }


    bool check_sequence_expr(const TemplateCombination &templateCombination, const std::vector<Component> &fixed,
                             const std::vector<int> &order) {
        bool flag;
        std::tm saved{};
        for (int k = 0; k < templateCombination.count && flag; k++) {
            Expiration temp_exp = templateCombination.legs[k].expiration;
            if (temp_exp.isbear()) {
                saved = fixed[order[k]].expiration;
            } else {
                if (temp_exp.sym == '+') {
                    Expiration prev_exp = templateCombination.legs[k - 1].expiration;
                    if (prev_exp < temp_exp &&
                        Time::comp(fixed[order[k - 1]].expiration, fixed[order[k]].expiration) <= 0) {
                        flag = false;
                        continue;
                    }
                }
                if (temp_exp.isdelta()) {
                    if (Time::comp(saved, fixed[order[k]].expiration) == -1) {
                        flag = false;
                        continue;
                    }
                    int test;
                    switch (temp_exp.sym) {
                        case 'q':
                            test = Time::diffInQuart(saved, fixed[order[k]].expiration);
                            break;
                        case 'm':
                            test = Time::diffInMonth(saved, fixed[order[k]].expiration);
                            break;
                        case 'd':
                            test = Time::diffInDays(saved, fixed[order[k]].expiration);
                            break;
                        case 'y':
                            test = Time::diffInYears(saved, fixed[order[k]].expiration);
                    }
                    if (temp_exp.count != test) {
                        flag = false;
                        continue;
                    }
                }
            }
        }
        return flag;
    }

    bool check_sequence_strike(const TemplateCombination &templateCombination, const std::vector<Component> &fixed,
                               const std::vector<int> &order) {
        bool flag;
        std::unordered_map<Strike, double> check;
        for (int k = 0; k < templateCombination.count && flag; k++) {
            if (Component::testOptions(fixed[order[k]].type)) {
                Strike temp_strike = templateCombination.legs[k].strike;
                if (temp_strike.ischeckable()) {
                    if (check[temp_strike] == 0) {
                        check[temp_strike] = fixed[order[k]].strike;
                    } else {
                        if (std::abs(check[temp_strike] - fixed[order[k]].strike) > 1e-7) {
                            flag = false;
                            continue;
                        }
                    }
                }
            }
        }
        return flag;
    }


    bool check_same_values_strike(const TemplateCombination &templateCombination, const std::vector<Component> &fixed,
                                  const std::vector<int> &order) {
        bool flag;
        double saved;
        for (int k = 0; k < templateCombination.count && flag; k++) {
            if (Component::testOptions(fixed[order[k]].type)) {
                Strike temp_strike = templateCombination.legs[k].strike;
                if (temp_strike.isbear()) {
                    saved = fixed[order[k]].strike;
                } else {
                    if (temp_strike.sym == '+') {
                        Strike prev_strike = templateCombination.legs[k - 1].strike;
                        if (prev_strike < temp_strike &&
                            fixed[order[k]].strike <= fixed[order[k - 1]].strike) {
                            flag = false;
                            continue;
                        }
                    }
                    if (temp_strike.sym == '-') {
                        Strike prev_strike = templateCombination.legs[k - 1].strike;
                        if (prev_strike < temp_strike &&
                            fixed[order[k]].strike >= fixed[order[k - 1]].strike) {
                            flag = false;
                            continue;
                        }
                    }
                }
            }
        }
        return flag;
    }

} // namespace

static std::map<std::string, CardinalityType> const string_to_cardinal = {{"fixed",    CardinalityType::FIXED},
                                                                          {"multiple", CardinalityType::MULTIPLE},
                                                                          {"more",     CardinalityType::MORE}};


bool
TemplateCombination::match(const TemplateCombination &templateCombination, const std::vector<Component> &combination,
                           std::vector<int> &order) {
    if (templateCombination.cardinalityType == CardinalityType::FIXED &&
        combination.size() != templateCombination.count) {
        return false;
    }
    if (templateCombination.cardinalityType == CardinalityType::MULTIPLE &&
        combination.size() % templateCombination.count != 0) {
        return false;
    }
    if (templateCombination.cardinalityType == CardinalityType::MORE &&
        combination.size() < templateCombination.count) {
        return false;
    }

    if (templateCombination.cardinalityType == CardinalityType::MORE) {
        bool flag = true;
        order.clear();
        for (int j = 0; j < combination.size(); j++) {
            flag = flag && TemplateComponent::match(templateCombination.legs[0], combination[j]);
            order.emplace_back(j + 1);
        }
        if (!flag) {
            order.clear();
        }
        return flag;
    }


    std::vector<Component> fixed = combination;
    if (templateCombination.cardinalityType == CardinalityType::MULTIPLE) {
        std::unordered_set<Component> temp(fixed.begin(), fixed.end());
        fixed = std::vector<Component>(temp.begin(), temp.end());
        if (fixed.size() != templateCombination.count) {
            return false;
        }
    }
    order.clear();
    for (int k = 0; k < templateCombination.count; k++) {
        order.emplace_back(k);
    }

    int end = 1;
    for (int i = 2; i <= templateCombination.count; end *= i++);
    for (int i = 0; i < end; i++) {
        bool flag = true;
        for (int j = 0; j < templateCombination.count; j++) {
            flag = flag && templateCombination.legs[j].match(fixed[order[j]]);
        }

        if (flag) {
            flag = check_same_values_expr(templateCombination, fixed, order);
        }

        if (flag) {
            flag = check_sequence_expr(templateCombination, fixed, order);
        }


        if (flag) {
            flag = check_same_values_strike(templateCombination, fixed, order);
        }

        if (flag) {
            flag = check_sequence_strike(templateCombination, fixed, order);
        }

        if (flag) {
            std::vector<int> temp = std::move(order);
            order.clear();
            std::vector<int> count(fixed.size(), 0);
            for (auto c: combination) {
                for (int k = 0; k < templateCombination.count; k++) {
                    if (c == fixed[temp[k]]) {
                        order.emplace_back(k + 1 + fixed.size() * count[k]++);
                        break;
                    }
                }
            }
            return true;
        }

        std::next_permutation(order.begin(), order.end());
    }
    return false;
}

int TemplateCombination::parse(pugi::xml_node node, TemplateCombination &templateCombination) {
    templateCombination.name = node.attribute("name").value();
    templateCombination.shortName = node.attribute("shortname").value();
    templateCombination.identifier = node.attribute("identifier").value();
    node = node.child("legs");
    if (auto it = string_to_cardinal.find(node.attribute("cardinality").value()); it != string_to_cardinal.end()) {
        templateCombination.cardinalityType = it->second;
    } else {
        return -1;
    }
    if (templateCombination.cardinalityType == CardinalityType::MORE) {
        templateCombination.count = std::stoi(node.attribute("mincount").value());
    }
    for (pugi::xml_node leg: node.children("leg")) {
        templateCombination.legs.emplace_back(TemplateComponent());
        if (!templateCombination.legs.back().parse(leg)) return -1;
    }
    if (templateCombination.cardinalityType != CardinalityType::MORE) {
        templateCombination.count = templateCombination.legs.size();
    }
    return templateCombination.count;
}

TemplateCombination::TemplateCombination(std::string &&name, CardinalityType cardinalityType, size_t count,
                                         std::vector<TemplateComponent> &&legs) : name(std::move(name)),
                                                                                  cardinalityType(cardinalityType),
                                                                                  count(count), legs(std::move(legs)) {}

TemplateCombination::TemplateCombination() {
    this->count = -1;
    this->cardinalityType = CardinalityType::FIXED;
    this->identifier = "";
    this->shortName = "";
    this->name = "";
}

bool TemplateCombination::match(const std::vector<Component> &combination, std::vector<int> &order) const {
    return match(*this, combination, order);
}

int TemplateCombination::parse(pugi::xml_node node) {
    return parse(node, *this);
}
