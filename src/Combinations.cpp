#include <iostream>
#include "Combinations.h"

bool Combinations::load(const std::filesystem::path &resource) {
    pugi::xml_document doc;
    if (!doc.load_file(resource.c_str())) return false;
    pugi::xml_node root = doc.child("combinations");
    for (auto node: root.children("combination")){
        TemplateCombination combination;
        if (TemplateCombination::parse(node, combination) == -1) return false;
        templates.emplace_back(combination);
    }
    return true;
}

std::string Combinations::classify(const std::vector<Component> &components, std::vector<int> &order) const {
    for (const auto& templateCombinations: templates) {
        order.clear();
        if (TemplateCombination::match(templateCombinations, components, order)) {
            std::string result = templateCombinations.name;
            return result;
        }
    }
    order.clear();
    return "Unclassified";
}

