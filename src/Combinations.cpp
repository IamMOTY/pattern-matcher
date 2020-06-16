#include <iostream>
#include "Combinations.h"

bool Combinations::load(const std::filesystem::path &resource) {
    pugi::xml_document doc;
    if (!doc.load_file(resource.c_str())) return false;
    pugi::xml_node root = doc.child("combinations");
    for (auto node: root.children("combination")){
        templates.emplace_back(TemplateCombination());
        if (templates.back().parse(node) == -1) return false;
    }
    return true;
}

std::string Combinations::classify(const std::vector<Component> &components, std::vector<int> &order) const {
    order.clear();
    for (const auto& templateCombinations: templates) {
        if (templateCombinations.match(components, order)) {
            std::string result = templateCombinations.name;
            return result;
        }
    }
    order.clear();
    return "Unclassified";
}
