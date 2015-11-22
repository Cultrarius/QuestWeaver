//
// Created by michael on 14.08.15.
//

#pragma once

#include <vector>
#include <memory>
#include "../QuestModel/Quest.h"
#include "../World/WorldModel.h"
#include "../Core/WeaverUtils.h"

namespace weave {
    class TemplateQuestProperty {
    public:
        TemplateQuestProperty(bool isMandatory, const std::string &name);

        bool IsMandatory() const;

        std::string GetName() const;

        bool operator==(const TemplateQuestProperty &other) const;

    private:
        bool isMandatory;
        std::string name;
    };

    class QuestPropertyValue {
    public:
        QuestPropertyValue(const TemplateQuestProperty &property, std::shared_ptr<WorldEntity> value);

        TemplateQuestProperty GetProperty() const;

        std::shared_ptr<WorldEntity> GetValue() const;

    private:
        TemplateQuestProperty property;
        std::shared_ptr<WorldEntity> value;
    };

    class TemplateQuestDescription {
    public:
        TemplateQuestDescription(const std::vector<std::string> &conditions, const std::string &text);

        bool SupportsConditions(const std::vector<std::string> &conditions) const;

        std::string GetText() const;

    private:
        std::vector<std::string> descriptionConditions;
        std::string text;
    };

    class Template {
    public:
        Template(std::string title,
                 std::vector<TemplateQuestProperty> properties,
                 std::vector<TemplateQuestDescription> descriptions);

        virtual std::shared_ptr<Quest> ToQuest(const std::vector<QuestPropertyValue> &questPropertyValues) const = 0;

        virtual std::vector<WorldModelAction> GetPropertyCandidates(const TemplateQuestProperty &property,
                                                                    const WorldModel &worldModel) const = 0;

        std::vector<TemplateQuestProperty> GetProperties() const;

    protected:
        std::string getBestFittingDescription(const std::vector<QuestPropertyValue> &questPropertyValues) const;

        std::string getTitle(const std::vector<QuestPropertyValue> &questPropertyValues) const;

    private:
        std::string title;
        std::vector<TemplateQuestProperty> properties;
        std::vector<TemplateQuestDescription> descriptions;
    };
}
