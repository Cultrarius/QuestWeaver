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

        RawStory CreateStory(std::map<std::string, std::shared_ptr<WorldEntity>>,
                             const WeaverGraph &graph) const override {
            return RawStory();
        }


    private:
        std::vector<std::string> requiredTypes;
    };
}
