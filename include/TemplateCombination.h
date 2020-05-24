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

struct TemplateCombination {
    TemplateCombination(std::string &&name, CardinalityType cardinalityType, size_t count,
                        std::vector<TemplateComponent> &&legs);

    TemplateCombination();

    static bool match(const TemplateCombination &templateCombination, const std::vector<Component> &combination,
                      std::vector<int> &order);

    bool match(const std::vector<Component> &combination,
               std::vector<int> &order) const;


    static int parse(pugi::xml_node node, TemplateCombination &templateCombination);

    int parse(pugi::xml_node node);


    std::string name;
    std::string shortName;
    std::string identifier;
    CardinalityType cardinalityType;
    int count{0};
    std::vector<TemplateComponent> legs;
};