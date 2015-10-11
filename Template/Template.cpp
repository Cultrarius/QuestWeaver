//
// Created by michael on 14.08.15.
//

#include "Template.h"
#include <algorithm>
#include "../Core/WeaverUtils.h"

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

TemplateQuestProperty::TemplateQuestProperty(bool isMandatory, const string &name) {
    this->isMandatory = isMandatory;
    this->name = name;
}

vector<TemplateQuestProperty> Template::GetProperties() const {
    return properties;
}

string TemplateQuestDescription::GetText() const {
    return text;
}

TemplateQuestDescription::TemplateQuestDescription(const vector<string> &conditions, const string &text) {
    this->conditions = conditions;
    sort(this->conditions.begin(), this->conditions.end());
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

QuestPropertyValue::QuestPropertyValue(const TemplateQuestProperty &property,
                                       shared_ptr<WorldEntity> value) :
        property(property), value(value) {
}

TemplateQuestProperty QuestPropertyValue::GetProperty() const {
    return property;
}

shared_ptr<WorldEntity> QuestPropertyValue::GetValue() const {
    return value;
}

std::string Template::getBestFittingDescription(const std::vector<QuestPropertyValue> &questPropertyValues) const {
    for (const auto &description : descriptions) {
        bool isFitting = true;
        for (const auto &questProperty : questPropertyValues) {
            const string &conditionName = questProperty.GetProperty().GetName();
            if (!description.SupportsCondition(conditionName)) {
                isFitting = false;
                break;
            }
        }
        if (isFitting) {
            string descriptionText = description.GetText();
            for (const auto &questProperty : questPropertyValues) {
                const string &conditionLabel = "%" + questProperty.GetProperty().GetName();
                const string &conditionValue = questProperty.GetValue()->toString();
                weave::replaceAll(&descriptionText, conditionLabel, conditionValue);
            }
            return descriptionText;
        }
    }
    // TODO add quest title to error message
    throw new runtime_error("Unable to find description fitting the supplied quest properties.");
}

bool TemplateQuestDescription::SupportsCondition(const string &condition) const {
    return binary_search(conditions.begin(), conditions.end(), condition);
}
