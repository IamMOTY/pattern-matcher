#pragma once

#include <vector>
#include <map>
#include "Component.h"
#include "TemplateComponent.h"
#include "Combinations.h"

enum class CardinalityType {
    FIXED,
    MULTIPLE,
    MORE
};

static std::map<std::string, CardinalityType> const string_to_cardinal = {{"fixed",    CardinalityType::FIXED},
                                                                          {"multiple", CardinalityType::MULTIPLE},
                                                                          {"more",     CardinalityType::MORE}};

struct TemplateCombination {
    TemplateCombination(std::string name, CardinalityType cardinalityType, size_t count,
                        std::vector<TemplateComponent> legs);

    TemplateCombination();

    static bool match(const TemplateCombination &templateCombination, const std::vector<Component> &combination,
                      std::vector<int> &order);

    static int parse(pugi::xml_node node, TemplateCombination &templateCombination);

    std::string name;
    std::string shortName;
    std::string identifier;
    CardinalityType cardinalityType;
    int count{0};
    std::vector<TemplateComponent> legs;

private:

};