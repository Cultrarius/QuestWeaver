//
// Created by michael on 04.02.16.
//

#pragma once

#include <Story/StoryTemplateFactory.h>

namespace weave {
    class TestStoryTemplateFactory : public StoryTemplateFactory {
    public:
        explicit TestStoryTemplateFactory(const std::string &testFolder) {
            this->testFolder = testFolder;
        }

        std::string GetNuggetFolder() const override {
            return testFolder;
        }

    protected:
        std::shared_ptr<StoryTemplate> createFromJsonValues(const Json::Value &root) const {
            return std::make_shared<StoryTemplate>();
        }

        const char *getTemplateFile() const {
            return "empty.st";
        }

    private:
        std::string testFolder;
    };
}
