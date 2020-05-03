#include <TemplateComponent.h>

#include <utility>


TemplateComponent::TemplateComponent(InstrumentType instrumentType, std::string strike,
                                     std::string expiration, std::string ratio) : instrumentType(
        instrumentType), strike(std::move(strike)), expiration(std::move(expiration)), ratio(std::move(ratio)) {}

int TemplateComponent::parse(pugi::xml_node node, TemplateComponent &templateComponent) {
    templateComponent.instrumentType = static_cast<InstrumentType>(node.attribute("type").value()[0]);
    templateComponent.ratio = node.attribute("ratio").value();
    if (node.attribute("strike_offset")) {
        templateComponent.strike = node.attribute("strike_offset").value();
    } else if (node.attribute("strike")) {
        templateComponent.strike = node.attribute("strike").value();
    } else {
        templateComponent.strike = "";
    }
    if (node.attribute("expiration_offset")) {
        templateComponent.expiration = node.attribute("expiration_offset").value();
    } else if (node.attribute("expiration")) {
        templateComponent.expiration = node.attribute("expiration").value();
    } else {
        templateComponent.expiration = "";
    }
    return 1;
}

TemplateComponent::TemplateComponent() {
    this->instrumentType = InstrumentType::Unknown;
    this->ratio = "";
    this->expiration = "";
    this->strike = "";
};

bool TemplateComponent::match(const TemplateComponent &templateComponent, Component component) {
    return !(templateComponent.instrumentType == InstrumentType::O &&
             component.type != InstrumentType::O &&
             component.type != InstrumentType::C &&
             component.type != InstrumentType::P ||
             templateComponent.instrumentType == InstrumentType::U &&
             component.type != InstrumentType::U &&
             component.type != InstrumentType::F ||
             templateComponent.instrumentType != component.type ||
             templateComponent.ratio == "-" &&
             component.ratio > 0 ||
             templateComponent.ratio == "+" &&
             component.ratio < 0 ||
             std::abs(std::stod(templateComponent.ratio) - component.ratio) > 1e-7);
}
