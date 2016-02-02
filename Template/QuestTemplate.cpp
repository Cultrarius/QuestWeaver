//
// Created by michael on 14.08.15.
//

#include <algorithm>
#include <Template/QuestTemplate.h>

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

vector<TemplateQuestProperty> QuestTemplate::GetProperties() const {
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

QuestTemplate::QuestTemplate(string title, vector<TemplateQuestProperty> properties,
                             vector<TemplateQuestDescription> descriptions, FormatterType formatterType) {
    this->title = title;
    this->properties = properties;
    this->descriptions = descriptions;
    this->formatterType = formatterType;
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

std::string QuestTemplate::getBestFittingDescription(const std::vector<QuestPropertyValue> &questPropertyValues) const {
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
                string conditionValue = questProperty.GetValue()->ToString();
                if (formatterType == FormatterType::HTML) {
                    vector<string> classes;
                    conditionValue = htmlEncloseWithTag(conditionValue, "span", getHtmlClasses(questProperty));
                }
                weave::replaceAll(&descriptionText, conditionLabel, conditionValue);
            }
            if (formatterType == FormatterType::HTML) {
                descriptionText = htmlEncloseWithTag(descriptionText, "span", "description");
            }
            return descriptionText;
        }
    }
    string message = "Unable to find description fitting the supplied quest properties. Quest Title: ";
    message += title;
    throw ContractFailedException(message);
}

bool TemplateQuestDescription::SupportsConditions(const std::vector<std::string> &conditions) const {
    return includes(conditions.begin(), conditions.end(),
                    descriptionConditions.begin(), descriptionConditions.end());
}

vector<string> QuestTemplate::getHtmlClasses(const QuestPropertyValue &questProperty) const {
    vector<string> classes;
    classes.push_back("entity");
    classes.push_back(questProperty.GetProperty().IsMandatory() ? "mandatory" : "optional");
    classes.push_back(questProperty.GetValue()->GetType());
    return classes;
}

std::string QuestTemplate::getTitle(const std::vector<QuestPropertyValue> &questPropertyValues) const {
    string titleText = this->title;
    for (const auto &questProperty : questPropertyValues) {
        const string &conditionLabel = "%" + questProperty.GetProperty().GetName();
        string conditionValue = questProperty.GetValue()->ToString();
        if (formatterType == FormatterType::HTML) {
            conditionValue = htmlEncloseWithTag(conditionValue, "span", getHtmlClasses(questProperty));
        }
        weave::replaceAll(&titleText, conditionLabel, conditionValue);
    }
    if (formatterType == FormatterType::HTML) {
        titleText = htmlEncloseWithTag(titleText, "span", "title");
    }
    return titleText;
}

bool TemplateQuestProperty::operator==(const TemplateQuestProperty &other) const {
    return name == other.name;
}

ID QuestTemplate::getEntityIdFromProperty(std::string propertyName,
                                          const std::vector<QuestPropertyValue> &questPropertyValues) {
    for (auto propertyValue : questPropertyValues) {
        if (propertyValue.GetProperty().GetName() == propertyName) {
            return propertyValue.GetValue()->GetId();
        }
    }
    return 0;
}

std::shared_ptr<Quest> QuestTemplate::ToQuest(const std::vector<QuestPropertyValue> &questPropertyValues) const {
    return ToQuest(questPropertyValues, "");
}
