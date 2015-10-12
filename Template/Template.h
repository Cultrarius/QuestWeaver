//
// Created by michael on 14.08.15.
//

#pragma once

#include <vector>
#include <memory>
#include "../QuestModel/Quest.h"
#include "../World/ModelAction.h"
#include "../World/WorldModel.h"
#include "../Core/WeaverUtils.h"

namespace weave {
    class TemplateQuestProperty {
    public:
        TemplateQuestProperty(bool isMandatory, const std::string &name);

        bool IsMandatory() const;

        std::string GetName() const;

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

        bool SupportsCondition(const std::string &condition) const;

        std::string GetText() const;

    private:
        std::vector<std::string> conditions;
        std::string text;
    };

    class Template {
    public:
        Template(std::vector<std::string> titles,
                 std::vector<TemplateQuestProperty> properties,
                 std::vector<TemplateQuestDescription> descriptions);

        virtual Quest ToQuest(const std::vector<QuestPropertyValue> &questPropertyValues) const = 0;

        virtual std::vector<ModelAction> GetPropertyCandidates(const TemplateQuestProperty &property,
                                                               const WorldModel &worldModel) const = 0;

        std::vector<std::string> GetTitles() const;

        std::vector<TemplateQuestProperty> GetProperties() const;

        std::vector<TemplateQuestDescription> GetDescriptions() const;

    protected:
        std::string getBestFittingDescription(const std::vector<QuestPropertyValue> &questPropertyValues) const;

    private:
        std::vector<std::string> titles;
        std::vector<TemplateQuestProperty> properties;
        std::vector<TemplateQuestDescription> descriptions;
    };
}
