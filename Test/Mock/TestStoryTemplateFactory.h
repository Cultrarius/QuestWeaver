//
// Created by michael on 04.02.16.
//

#pragma once

#include <Story/StoryTemplateFactory.h>
#include "TestStoryTemplate.h"

namespace weave {
    class TestStoryTemplateFactory : public StoryTemplateFactory {
    public:
        explicit TestStoryTemplateFactory(const std::string &testFolder) {
            this->testFolder = testFolder;
        }

        TestStoryTemplateFactory(const std::string &testFolder, std::vector<std::string> requiredTypes) {
            this->testFolder = testFolder;
            this->requiredTypes = requiredTypes;
        }

        std::string GetNuggetFolder() const override {
            return testFolder;
        }

    protected:
        std::shared_ptr<StoryTemplate> createFromJsonValues(const Json::Value &root) const override {
            return std::make_shared<TestStoryTemplate>(requiredTypes);
        }

        const char *getTemplateFile() const override {
            return "empty.st";
        }

    private:
        std::string testFolder;
        std::vector<std::string> requiredTypes;
    };
}
