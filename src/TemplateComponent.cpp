#include <TemplateComponent.h>

#include <utility>


TemplateComponent::TemplateComponent(InstrumentType instrumentType, std::string &&strike,
                                     std::string &&expiration, std::string &&ratio) : instrumentType(
        instrumentType), strike(std::move(strike)), expiration(std::move(expiration)), ratio(std::move(ratio)) {}

TemplateComponent::TemplateComponent() {
    this->instrumentType = InstrumentType::Unknown;
    this->expiration = Expiration("");
    this->strike = Strike("");
    this->ratio;
};


bool TemplateComponent::match(const Component &component) const {
    if (this->instrumentType == InstrumentType::O) {
        if (component.type != InstrumentType::O &&
            component.type != InstrumentType::C &&
            component.type != InstrumentType::P) {
            return false;
        }
    } else if (this->instrumentType == InstrumentType::U) {
        if (component.type != InstrumentType::U &&
            component.type != InstrumentType::F) {
            return false;
        }
    } else if (this->instrumentType != component.type) {
        return false;
    }

    return this->ratio.match(component.ratio);
}

bool TemplateComponent::parse(const pugi::xml_node & node) {
    this->instrumentType = static_cast<InstrumentType>(node.attribute("type").value()[0]);
    this->ratio = Ratio(node.attribute("ratio").value());
    if (node.attribute("strike_offset")) {
        this->strike = Strike(node.attribute("strike_offset").value());
    } else if (node.attribute("strike")) {
        this->strike = Strike(node.attribute("strike").value());
    } else {
        this->strike = Strike("");
    }
    if (node.attribute("expiration_offset")) {
        this->expiration = Expiration(node.attribute("expiration_offset").value());
    } else if (node.attribute("expiration")) {
        this->expiration = Expiration(node.attribute("expiration").value());
    } else {
        this->expiration = Expiration("");
    }
    return true;
}

