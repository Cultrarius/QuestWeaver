//
// Created by michael on 05.02.16.
//

#pragma once

#include <Story/StoryTemplate.h>

namespace weave {
    class TestStoryTemplate : public StoryTemplate {
    public:
        explicit TestStoryTemplate(std::vector<std::string> requiredTypes) {
            this->requiredTypes = requiredTypes;
        }

        std::vector<std::string> GetRequiredEntities() const override {
            return requiredTypes;
        }

        std::vector<StoryLine> CreateStory(std::map<std::string, std::shared_ptr<WorldEntity>>,
                                           const WeaverGraph &graph) const override {
            return std::vector<StoryLine>();
        }


    private:
        std::vector<std::string> requiredTypes;
    };
}
