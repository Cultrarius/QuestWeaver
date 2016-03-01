//
// Created by michael on 05.02.16.
//

#pragma once

#include <Story/StoryTemplate.h>

namespace weave {
    class AgentIntroStoryTemplate : public StoryTemplate {
    public:
        explicit AgentIntroStoryTemplate(std::vector<RawStoryLine> rawLines) : StoryTemplate(rawLines) { }

        std::set<std::string> GetRequiredEntities() const override {
            return requiredTypes;
        }

        StoryTemplateResult CreateStory(EntityMap requiredEntities, const WeaverGraph &graph) const override;

    private:
        std::set<std::string> requiredTypes = {"agent"};
    };

}
