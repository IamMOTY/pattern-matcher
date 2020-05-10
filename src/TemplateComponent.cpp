#include <TemplateComponent.h>

#include <utility>


TemplateComponent::TemplateComponent(InstrumentType instrumentType, std::string &&strike,
                                     std::string &&expiration, std::string &&ratio) : instrumentType(
        instrumentType), strike(std::move(strike)), expiration(std::move(expiration)), ratio(std::move(ratio)) {}

bool TemplateComponent::parse(const pugi::xml_node &node, TemplateComponent &templateComponent) {
    templateComponent.instrumentType = static_cast<InstrumentType>(node.attribute("type").value()[0]);
    templateComponent.ratio = Ratio(node.attribute("ratio").value());
    if (node.attribute("strike_offset")) {
        templateComponent.strike = Strike(node.attribute("strike_offset").value());
    } else if (node.attribute("strike")) {
        templateComponent.strike = Strike(node.attribute("strike").value());
    } else {
        templateComponent.strike = Strike("");
    }
    if (node.attribute("expiration_offset")) {
        templateComponent.expiration = Expiration(node.attribute("expiration_offset").value());
    } else if (node.attribute("expiration")) {
        templateComponent.expiration = Expiration(node.attribute("expiration").value());
    } else {
        templateComponent.expiration = Expiration("");
    }
    return true;
}



TemplateComponent::TemplateComponent() {
    this->instrumentType = InstrumentType::Unknown;
    this->expiration = Expiration("");
    this->strike = Strike("");
    this->ratio;
};

bool TemplateComponent::match(const TemplateComponent &templateComponent, const Component &component) {
    if (templateComponent.instrumentType == InstrumentType::O) {
        if (component.type != InstrumentType::O &&
            component.type != InstrumentType::C &&
            component.type != InstrumentType::P) {
            return false;
        }
    } else if (templateComponent.instrumentType == InstrumentType::U) {
        if (component.type != InstrumentType::U &&
            component.type != InstrumentType::F) {
            return false;
        }
    } else if (templateComponent.instrumentType != component.type) {
        return false;
    }

   return templateComponent.ratio.match(component.ratio);
}

bool TemplateComponent::match(const Component &component) const {
    return match(*this, component);
}

bool TemplateComponent::parse(const pugi::xml_node & node) {
    return parse(node, *this);
}

