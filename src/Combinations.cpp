#include "Combinations.h"

bool Combinations::load(const std::filesystem::path &resource) {
    pugi::xml_document doc;
    if (!doc.load_file(resource.c_str())) return false;
    for (pugi::xml_node node: doc.children("combination")){
        TemplateCombination combination;
        if (TemplateCombination::parse(node, combination) == -1) return false;
        templates.emplace_back(combination);
    }
    return true;
}

std::string Combinations::classify(const std::vector<Component> &components, std::vector<int> &order) const {
    for (const auto& templateCombinations: templates) {
        if (TemplateCombination::match(templateCombinations, components, order)) {
            std::string result = templateCombinations.name + "\n";
            for (int i: order) {
                result += std::to_string(i) + "\n";
            }
            return result;
        }
    }
    order.clear();
    return "Unclassified";
}

