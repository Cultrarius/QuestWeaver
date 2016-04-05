//
// Created by michael on 14.08.15.
//

#include <algorithm>
#include <Template/QuestTemplate.h>

using namespace std;
using namespace weave;

PropertyCandidate::PropertyCandidate() noexcept { }

PropertyCandidate::PropertyCandidate(WorldModelAction singleAction) noexcept :
        actions({singleAction}), candidateValue(singleAction.GetEntity()) { }

PropertyCandidate::PropertyCandidate(vector<WorldModelAction> actionList,
                                     shared_ptr<WorldEntity> candidateValue) noexcept :
        actions(actionList), candidateValue(candidateValue) { }

std::shared_ptr<WorldEntity> PropertyCandidate::GetValue() const noexcept {
    return candidateValue;
}

std::vector<WorldModelAction> PropertyCandidate::GetActions() const noexcept {
    return actions;
}

vector<TemplateQuestProperty> QuestTemplate::GetProperties() const noexcept {
    return properties;
}

bool QuestTemplate::IsValid(const WorldModel &, const QuestModel &) const noexcept {
    return true;
}

bool QuestTemplate::HasPriority() const noexcept {
    return false;
}

string TemplateQuestDescription::GetText(FormatterType format) const noexcept {
    if (format == FormatterType::HTML) {
        return htmlEncloseWithTag(text, "span", "description");
    }
    return text;
}

TemplateQuestDescription::TemplateQuestDescription(const vector<string> &conditions, const string &text) noexcept {
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

string QuestTemplate::getBestFittingDescription(const vector<QuestPropertyValue> &questPropertyValues) const {
    vector<string> sortedConditions;
    for (auto propertyValue : questPropertyValues) {
        sortedConditions.push_back(propertyValue.GetProperty().GetName());
    }
    sort(sortedConditions.begin(), sortedConditions.end());
    for (const auto &description : descriptions) {
        if (description.SupportsConditions(sortedConditions)) {
            string descriptionText = description.GetText(formatterType);
            for (const auto &questProperty : questPropertyValues) {
                string conditionLabel = "%" + questProperty.GetProperty().GetName();
                string conditionValue = questProperty.GetValueString(formatterType);
                weave::replaceAll(&descriptionText, conditionLabel, conditionValue);

                for (auto entry : resolveAdditionalPlaceholders(questProperty)) {
                    conditionValue = questProperty.GetValueString(formatterType);
                    weave::replaceAll(&descriptionText, "%" + entry.first, entry.second);
                }
            }
            return descriptionText;
        }
    }
    string message = "Unable to find description fitting the supplied quest properties.\nQuest Title: ";
    message += title;
    throw ContractFailedException(message);
}

bool TemplateQuestDescription::SupportsConditions(const vector<string> &conditions) const noexcept {
    return includes(conditions.begin(), conditions.end(),
                    descriptionConditions.begin(), descriptionConditions.end());
}

string QuestTemplate::getTitle(const vector<QuestPropertyValue> &questPropertyValues) const noexcept {
    string titleText = this->title;
    for (const auto &questProperty : questPropertyValues) {
        const string &conditionLabel = "%" + questProperty.GetProperty().GetName();
        string conditionValue = questProperty.GetValueString(formatterType);
        weave::replaceAll(&titleText, conditionLabel, conditionValue);
    }
    if (formatterType == FormatterType::HTML) {
        titleText = htmlEncloseWithTag(titleText, "span", "title");
    }
    return titleText;
}

map<string, string> QuestTemplate::resolveAdditionalPlaceholders(QuestPropertyValue) const noexcept {
    return map<string, string>();
}

ID QuestTemplate::getEntityIdFromProperty(string propertyName,
                                          const vector<QuestPropertyValue> &questPropertyValues) noexcept {
    for (auto propertyValue : questPropertyValues) {
        if (propertyValue.GetProperty().GetName() == propertyName) {
            return propertyValue.GetValue()->GetId();
        }
    }
    return 0;
}

shared_ptr<Quest> QuestTemplate::ToQuest(const vector<QuestPropertyValue> &questPropertyValues) const {
    return ToQuest(questPropertyValues, "");
}
