#pragma once

#include "Strike.h"
#include "Component.h"
#include "Ratio.h"
#include "pugixml.hpp"
#include "Expiration.h"
#include <vector>
#include <variant>


struct TemplateComponent {
    TemplateComponent();

    TemplateComponent(InstrumentType instrumentType, std::string &&strike, std::string &&expiration,
                      std::string &&ratio);

    [[nodiscard]] bool match(const Component&) const;
    bool parse(const pugi::xml_node&);

    InstrumentType instrumentType;
    Strike strike;
    Expiration expiration;
    Ratio ratio;
};