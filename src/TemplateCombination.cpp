#include <utility>
#include <set>
#include <algorithm>
#include <bits/unordered_set.h>
#include "Time.h"
#include "TemplateCombination.h"


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
    for(int k = 0; k < templateCombination.count; k++) {
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
            std::tm def{};
            std::map<std::string, std::tm> check;
            for (int k = 0; k < templateCombination.count && flag; k++) {
                std::string templateString = templateCombination.legs[k].expiration;
                if (templateString.size() == 1 && isupper(templateString[0]) ||
                    !templateString.empty() && templateString[0] == '+') {
                    if (Time::comp(check[templateString], def) == 0) {
                        check[templateString] = fixed[order[k]].expiration;
                    } else {
                        if (Time::comp(check[templateString], fixed[order[k]].expiration) != 0) {
                            flag = false;
                            continue;
                        }
                    }
                }
            }
        }

        if (flag) {
            std::tm saved{};
            for (int k = 0; k < templateCombination.count && flag; k++) {
                std::string templateString = templateCombination.legs[k].expiration;
                if (templateString.size() == 1 && isupper(templateString[0]) || templateString.empty()) {
                    saved = fixed[order[k]].expiration;
                } else {
                    if (templateString[0] == '+') {
                        std::string prevTemplateString = templateCombination.legs[k - 1].expiration;
                        if ((prevTemplateString.size() == 1 && isupper(prevTemplateString[0]) ||
                             prevTemplateString.empty() || prevTemplateString < templateString) &&
                            Time::comp(fixed[order[k - 1]].expiration, fixed[order[k]].expiration) <= 0) {
                            flag = false;
                            continue;
                        }
                    }
                    if (templateString.back() == 'q' || templateString.back() == 'm' ||
                        templateString.back() == 'y' || templateString.back() == 'd') {
                        if (Time::comp(saved, fixed[order[k]].expiration) == -1) {
                            flag = false;
                            continue;
                        }
                        std::string temp;
                        for (char c:templateString) {
                            if (isdigit(c)) {
                                temp += c;
                            }
                        }
                        if (temp.empty()) {
                            temp = "1";
                        }
                        int count = std::stoi(temp);
                        int test;
                        switch (templateString.back()) {
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
                        if (count != test) {
                            flag = false;
                            continue;
                        }
                    }
                }
            }
        }


        if (flag) {
            std::map<std::string, double> check;
            for (int k = 0; k < templateCombination.count && flag; k++) {
                if (Component::testOptions(fixed[order[k]].type)) {
                    std::string templateString = templateCombination.legs[k].strike;
                    if (templateString.size() == 1 && isupper(templateString[0]) ||
                        !templateString.empty() && templateString[0] == '+' || templateString[0] == '-') {
                        if (check[templateString] == 0) {
                            check[templateString] = fixed[order[k]].strike;
                        } else {
                            if (std::abs(check[templateString] - fixed[order[k]].strike) > 1e-7) {
                                flag = false;
                                continue;
                            }
                        }
                    }
                }
            }
        }

        if (flag) {
            double saved;
            for (int k = 0; k < templateCombination.count && flag; k++) {
                if (Component::testOptions(fixed[order[k]].type)) {
                    std::string templateString = templateCombination.legs[k].strike;
                    if (templateString.size() == 1 && isupper(templateString[0]) || templateString.empty()) {
                        saved = fixed[order[k]].strike;
                    } else {
                        if (templateString[0] == '+') {
                            std::string prevTemplateString = templateCombination.legs[k - 1].strike;
                            if ((prevTemplateString.size() == 1 && isupper(prevTemplateString[0]) ||
                                 prevTemplateString.empty() || prevTemplateString < templateString) &&
                                fixed[order[k]].strike <= fixed[order[k - 1]].strike) {
                                flag = false;
                                continue;
                            }
                        }
                        if (templateString[0] == '-') {
                            std::string prevTemplateString = templateCombination.legs[k - 1].strike;
                            if ((prevTemplateString.size() == 1 && isupper(prevTemplateString[0]) ||
                                 prevTemplateString.empty() || prevTemplateString < templateString) &&
                                fixed[order[k]].strike >= fixed[order[k - 1]].strike) {
                                flag = false;
                                continue;
                            }
                        }
                    }
                }
            }
        }

        if (flag) {
            std::vector<int> temp = std::move(order);
            order.clear();
            std::vector<int> count(fixed.size(),0);
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

TemplateCombination::TemplateCombination(std::string && name, CardinalityType cardinalityType, size_t count,
                                         std::vector<TemplateComponent> && legs) : name(std::move(name)),
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