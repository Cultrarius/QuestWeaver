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

        TestStoryTemplateFactory(const std::string &testFolder, std::set<std::string> requiredTypes) {
            this->testFolder = testFolder;
            this->requiredTypes = requiredTypes;
        }

        TestStoryTemplateFactory(const std::string &testFolder, const std::string &templateFile) {
            this->testFolder = testFolder;
            this->templateFile = templateFile;
        }

        TestStoryTemplateFactory(const std::string &testFolder, const std::string &templateFile,
                                 std::vector<WorldModelAction> actions) {
            this->testFolder = testFolder;
            this->templateFile = templateFile;
            this->actions = actions;
        }

        std::string GetNuggetFolder() const override {
            return testFolder;
        }

        bool TemplatesReturnInvalidIDs = false;

    protected:
        std::shared_ptr<StoryTemplate> createFromJsonValues(const Json::Value &root) const override {
            std::shared_ptr<TestStoryTemplate> storyTemplate;
            if (templateFile.empty()) {
                storyTemplate = std::make_shared<TestStoryTemplate>(requiredTypes, std::vector<RawStoryLine>(),
                                                                    actions);
            } else {
                storyTemplate = std::make_shared<TestStoryTemplate>(readRequired(root), readRawLines(root), actions);
            }
            storyTemplate->ReturnInvalidIDs = TemplatesReturnInvalidIDs;
            return storyTemplate;
        }

        const char *getTemplateFile() const override {
            return templateFile.empty() ? "empty.st" : templateFile.c_str();
        }

    private:
        std::string testFolder;
        std::string templateFile;
        std::set<std::string> requiredTypes;
        std::vector<WorldModelAction> actions;
    };
}
