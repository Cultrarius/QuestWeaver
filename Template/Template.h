//
// Created by michael on 14.08.15.
//

#pragma once

#include <vector>
#include "../QuestModel/Quest.h"

class TemplateQuestProperty {
public:
    TemplateQuestProperty(bool isMandatory, std::string name);

    bool IsMandatory() const;

    std::string GetName() const;

private:
    bool isMandatory;
    std::string name;
};

class TemplateQuestDescription {
public:
    TemplateQuestDescription(std::vector<std::string> conditions, std::string text);

    std::vector<std::string> GetConditions() const;

    std::string GetText() const;

private:
    std::vector<std::string> conditions;
    std::string text;
};

class Template {
public:
    virtual Quest ToQuest() const = 0;

    Template(std::vector<std::string> titles,
             std::vector<TemplateQuestProperty> properties,
             std::vector<TemplateQuestDescription> descriptions);

    std::vector<std::string> GetTitles() const;

    std::vector<TemplateQuestProperty> GetProperties() const;

    std::vector<TemplateQuestDescription> GetDescriptions() const;

private:
    std::vector<std::string> titles;
    std::vector<TemplateQuestProperty> properties;
    std::vector<TemplateQuestDescription> descriptions;
};


