#pragma once

#include "Component.h"
#include "pugixml.hpp"
#include <vector>
#include <variant>


struct TemplateComponent {
    TemplateComponent();

    TemplateComponent(InstrumentType instrumentType, std::string &&strike, std::string &&expiration,
                      std::string &&ratio);

    static bool match(const TemplateComponent&,const Component&);
    [[nodiscard]] bool match(const Component&) const;
    static bool parse(const pugi::xml_node&, TemplateComponent&);
    bool parse(const pugi::xml_node&);

    InstrumentType instrumentType;
    std::string strike;
    std::string expiration;
    std::string ratio;
};