//
// Created by michael on 15.08.15.
//

#pragma once

#include "../TemplateFactory.h"

namespace weave {
    class SpaceQuestTemplateFactory : public TemplateFactory {
    public:
        explicit SpaceQuestTemplateFactory(std::shared_ptr<RandomStream> randomStream);

        SpaceQuestTemplateFactory(std::shared_ptr<RandomStream> randomStream, Directories directories);

    protected:
        std::shared_ptr<Template> createFromJsonValues(const Json::Value &root) const;

        const char *getTemplateFile() const;
    };
}

