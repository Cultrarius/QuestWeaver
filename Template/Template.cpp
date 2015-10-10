//
// Created by michael on 14.08.15.
//

#include "Template.h"

using namespace std;

vector<string> Template::GetTitles() const {
    return titles;
}

bool TemplateQuestProperty::IsMandatory() const {
    return isMandatory;
}

string TemplateQuestProperty::GetName() const {
    return name;
}

TemplateQuestProperty::TemplateQuestProperty(bool isMandatory, string& name) {
    this->isMandatory = isMandatory;
    this->name = name;
}

vector<TemplateQuestProperty> Template::GetProperties() const {
    return properties;
}

vector<string> TemplateQuestDescription::GetConditions() const {
    return conditions;
}

string TemplateQuestDescription::GetText() const {
    return text;
}

TemplateQuestDescription::TemplateQuestDescription(vector<string> conditions, string text) {
    this->conditions = conditions;
    this->text = text;
}

vector<TemplateQuestDescription> Template::GetDescriptions() const {
    return descriptions;
}

Template::Template(vector<string> titles, vector<TemplateQuestProperty> properties,
                   vector<TemplateQuestDescription> descriptions) {
    this->titles = titles;
    this->properties = properties;
    this->descriptions = descriptions;
}

QuestPropertyValue::QuestPropertyValue(TemplateQuestProperty &property,
                                       shared_ptr<WorldEntity> value) :
        property(property), value(value) {
}

TemplateQuestProperty QuestPropertyValue::GetProperty() const {
    return property;
}

shared_ptr<WorldEntity> QuestPropertyValue::GetValue() const {
    return value;
}
