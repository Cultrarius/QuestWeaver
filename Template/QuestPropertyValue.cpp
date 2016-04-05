//
// Created by michael on 05.04.16.
//

#include "Template/QuestPropertyValue.h"

using namespace std;
using namespace weave;

QuestPropertyValue::QuestPropertyValue(const TemplateQuestProperty &property,
                                       shared_ptr<WorldEntity> value) noexcept :
        property(property), value(value) {
}

TemplateQuestProperty QuestPropertyValue::GetProperty() const noexcept {
    return property;
}

shared_ptr<WorldEntity> QuestPropertyValue::GetValue() const noexcept {
    return value;
}

string QuestPropertyValue::GetValueString(FormatterType format) const noexcept {
    return GetValueString(value, property.IsMandatory(), format);
}

string QuestPropertyValue::GetValueString(shared_ptr<WorldEntity> value, bool isMandatory, FormatterType format) {
    if (!value) {
        return "";
    }
    if (format == FormatterType::HTML) {
        vector<string> classes;
        classes.push_back("entity");
        classes.push_back(isMandatory ? "mandatory" : "optional");
        classes.push_back(value->GetType());
        return htmlEncloseWithTag(value->ToString(), "span", classes);
    }
    return value->ToString();
}

bool TemplateQuestProperty::IsMandatory() const noexcept {
    return isMandatory;
}

string TemplateQuestProperty::GetName() const noexcept {
    return name;
}

TemplateQuestProperty::TemplateQuestProperty(bool isMandatory, const string &name) noexcept {
    this->isMandatory = isMandatory;
    this->name = name;
}
