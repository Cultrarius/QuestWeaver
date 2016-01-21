//
// Created by michael on 15.08.15.
//

#pragma once

#include "../TemplateFactory.h"

namespace weave {
    class SpaceQuestTemplateFactory : public TemplateFactory {
    protected:
        virtual std::shared_ptr<Template> createFromJsonValues(const Json::Value &root) const;

        virtual const char *getTemplateFile() const;
    };
}
