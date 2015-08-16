//
// Created by michael on 14.08.15.
//

#include "Template.h"

std::vector<std::string> Template::GetTitles() const {
    return titles;
}

bool TemplateQuestProperty::IsMandatory() const {
    return isMandatory;
}

std::string TemplateQuestProperty::GetName() const {
    return name;
}

TemplateQuestProperty::TemplateQuestProperty(bool isMandatory, std::string name) {
    this->isMandatory = isMandatory;
    this->name = name;
}

std::vector<TemplateQuestProperty> Template::GetProperties() const {
    return properties;
}

std::vector<std::string> TemplateQuestDescription::GetConditions() const {
    return conditions;
}

std::string TemplateQuestDescription::GetText() const {
    return text;
}

TemplateQuestDescription::TemplateQuestDescription(std::vector<std::string> conditions, std::string text) {
    this->conditions = conditions;
    this->text = text;
}

std::vector<TemplateQuestDescription> Template::GetDescriptions() const {
    return descriptions;
}

Template::Template(std::vector<std::string> titles, std::vector<TemplateQuestProperty> properties,
                   std::vector<TemplateQuestDescription> descriptions) {
    this->titles = titles;
    this->properties = properties;
    this->descriptions = descriptions;
}
