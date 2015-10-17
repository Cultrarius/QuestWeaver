//
// Created by michael on 14.08.15.
//

#pragma once

#include "../Template.h"

namespace weave {
    class ExploreRegionTemplate : public Template {
    public:
        std::shared_ptr<Quest> ToQuest(const std::vector<QuestPropertyValue> &questPropertyValues) const;

        std::vector<ModelAction> GetPropertyCandidates(const TemplateQuestProperty &property,
                                                       const WorldModel &worldModel) const;

        ExploreRegionTemplate(std::string title,
                              std::vector<TemplateQuestProperty> properties,
                              std::vector<TemplateQuestDescription> descriptions);
    };
}

