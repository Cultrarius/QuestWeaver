//
// Created by michael on 15.08.15.
//

#pragma once

#include "Template/QuestTemplateFactory.h"

namespace weave {
    class SpaceQuestTemplateFactory : public QuestTemplateFactory {
    protected:
        std::shared_ptr<QuestTemplate> createFromJsonValues(const Json::Value &root) const override;

        std::vector<const char *> getTemplateFiles() const override;
    };
}
