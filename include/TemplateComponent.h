#pragma once

#include "Component.h"
#include <pugixml.hpp>
#include <vector>


struct TemplateComponent {
    TemplateComponent();

    TemplateComponent(InstrumentType instrumentType, std::string strike, std::string expiration,
                      std::string ratio);

    static bool match(const TemplateComponent&, Component);
    static int parse(pugi::xml_node, TemplateComponent&);
    InstrumentType instrumentType;
    std::string strike;
    std::string expiration;
    std::string ratio;
};