//
// Created by michael on 01.02.16.
//

#pragma once

#include <Story/StoryTemplate.h>
#include <Story/StoryTemplateFactory.h>

namespace weave {
    class CommonSpaceStoryFactory : public StoryTemplateFactory {
    public:
        ~CommonSpaceStoryFactory() = default;

        std::string GetNuggetFolder() const override {
            return "Space";
        }

    protected:
        std::shared_ptr<StoryTemplate> createFromJsonValues(const Json::Value &root) const override;

        const char *getTemplateFile() const override;
    };
}
