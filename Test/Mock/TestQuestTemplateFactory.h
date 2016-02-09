//
// Created by Michael on 10.02.2016.
//

#pragma once

#include <Template/QuestTemplateFactory.h>

namespace weave {
    class TestQuestTemplateFactory : public QuestTemplateFactory {
    public:
        TestQuestTemplateFactory(std::string templateFile);

        std::shared_ptr<QuestTemplate> createFromJsonValues(const Json::Value &root) const override;

        const char *getTemplateFile() const override;

    private:
        std::string templateFile;
    };
}