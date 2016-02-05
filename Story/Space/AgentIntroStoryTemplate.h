//
// Created by michael on 05.02.16.
//

#pragma once

#include <Story/StoryTemplate.h>

namespace weave {
    class AgentIntroStoryTemplate : public StoryTemplate {
    public:
        std::vector<std::string> GetRequiredEntities() const override {
            return requiredTypes;
        }

    private:
        std::vector<std::string> requiredTypes = {"agent"};
    };
}
