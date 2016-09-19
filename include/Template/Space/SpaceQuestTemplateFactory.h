//
// Created by michael on 15.08.15.
//

#pragma once

#include "Template/QuestTemplateFactory.h"

namespace weave {
    class SpaceQuestTemplateFactory : public QuestTemplateFactory {
    public:
        SpaceQuestTemplateFactory() = default;

        SpaceQuestTemplateFactory(SpaceQuestTemplateFactory &&) = default;
    protected:
        std::shared_ptr<QuestTemplate> createFromJsonValues(const Json::Value &root) const override;

        std::vector<const char *> getTemplateFiles() const override;
    private:
        SpaceQuestTemplateFactory(const SpaceQuestTemplateFactory&) = delete;
    };
}
