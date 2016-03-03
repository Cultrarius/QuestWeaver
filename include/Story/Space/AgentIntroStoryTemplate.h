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

        StoryTemplateResult CreateStory(const EntityMap &requiredEntities, const WeaverGraph &graph,
                                        const WorldModel &worldModel) const override;

        bool IsValid(const EntityMap &requiredEntities, const WeaverGraph &graph,
                     const WorldModel &worldModel) const override;

    private:
        std::set<std::string> requiredTypes = {"agent"};
        std::string metaDataMarker = "introStoryDone";

        std::vector<std::shared_ptr<WorldEntity>> getValidEntities(const EntityMap &entityMap,
                                                                   const WorldModel &worldModel) const;
    };

}
