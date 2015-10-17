//
// Created by michael on 14.08.15.
//

#include "Template.h"
#include <algorithm>

using namespace std;
using namespace weave;

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
    this->descriptionConditions = conditions;
    sort(this->descriptionConditions.begin(), this->descriptionConditions.end());
    this->text = text;
}

Template::Template(string title, vector<TemplateQuestProperty> properties,
                   vector<TemplateQuestDescription> descriptions) {
    this->title = title;
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
    std::vector<std::string> sortedConditions;
    for (auto propertyValue : questPropertyValues) {
        sortedConditions.push_back(propertyValue.GetProperty().GetName());
    }
    sort(sortedConditions.begin(), sortedConditions.end());
    for (const auto &description : descriptions) {
        if (description.SupportsConditions(sortedConditions)) {
            string descriptionText = description.GetText();
            for (const auto &questProperty : questPropertyValues) {
                const string &conditionLabel = "%" + questProperty.GetProperty().GetName();
                const string &conditionValue = questProperty.GetValue()->ToString();
                weave::replaceAll(&descriptionText, conditionLabel, conditionValue);
            }
            return descriptionText;
        }
    }
    // TODO add quest title to error message
    throw ContractFailedException("Unable to find description fitting the supplied quest properties.");
}

bool TemplateQuestDescription::SupportsConditions(const std::vector<std::string> &conditions) const {
    return includes(conditions.begin(), conditions.end(),
                    descriptionConditions.begin(), descriptionConditions.end());
}

std::string Template::getTitle(const std::vector<QuestPropertyValue> &questPropertyValues) const {
    string titleText = this->title;
    for (const auto &questProperty : questPropertyValues) {
        const string &conditionLabel = "%" + questProperty.GetProperty().GetName();
        const string &conditionValue = questProperty.GetValue()->ToString();
        weave::replaceAll(&titleText, conditionLabel, conditionValue);
    }
    return titleText;
}
