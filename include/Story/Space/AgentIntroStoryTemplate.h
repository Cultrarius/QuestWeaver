//
// Created by michael on 05.02.16.
//

#pragma once

#include <Story/StoryTemplate.h>

namespace weave {
    class AgentIntroStoryTemplate : public StoryTemplate {
    public:
        AgentIntroStoryTemplate(std::vector<RawStoryLine> rawLines) : StoryTemplate(rawLines) { }

        std::vector<std::string> GetRequiredEntities() const override {
            return requiredTypes;
        }

        std::vector<StoryLine> CreateStory(std::map<std::string, std::shared_ptr<WorldEntity>>,
                                           const WeaverGraph &graph) const override;

    private:
        std::vector<std::string> requiredTypes = {"agent"};
    };

}
