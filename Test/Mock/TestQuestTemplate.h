//
// Created by michael on 09.02.16.
//

#pragma once

#include <Template/QuestTemplate.h>

namespace weave {
    class TestQuestTemplate : public QuestTemplate {
    public:
        TestQuestTemplate(std::vector<TemplateQuestProperty> properties,
                          std::vector<TemplateQuestDescription> descriptions);

        std::shared_ptr<Quest> ToQuest(const std::vector<QuestPropertyValue> &questPropertyValues,
                                       const std::string &questStory) const override;

        std::vector<WorldModelAction> GetPropertyCandidates(const TemplateQuestProperty &property,
                                                            const WorldModel &worldModel) const override;
    };
}