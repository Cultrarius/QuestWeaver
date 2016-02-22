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

        TestStoryTemplateFactory(const std::string &testFolder, const std::string &templateFile) {
            this->testFolder = testFolder;
            this->templateFile = templateFile;
        }

        std::string GetNuggetFolder() const override {
            return testFolder;
        }

    protected:
        std::shared_ptr<StoryTemplate> createFromJsonValues(const Json::Value &root) const override {
            return std::make_shared<TestStoryTemplate>(requiredTypes);
        }

        const char *getTemplateFile() const override {
            return templateFile.empty() ? "empty.st" : templateFile.c_str();
        }

    private:
        std::string testFolder;
        std::string templateFile;
        std::vector<std::string> requiredTypes;
    };
}
