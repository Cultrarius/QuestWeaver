//
// Created by michael on 05.02.16.
//

#pragma once

#include <Story/StoryTemplate.h>

namespace weave {
    class AgentIntroStoryTemplate : public StoryTemplate {
    public:
        explicit AgentIntroStoryTemplate(std::string rawText) : StoryTemplate(rawText, {"agent"}) { }

        StoryTemplateResult CreateStory(const EntityMap &requiredEntities, const WeaverGraph &graph,
                                        const WorldModel &worldModel) const override;

        bool IsValid(const EntityMap &requiredEntities, const WeaverGraph &graph,
                     const WorldModel &worldModel) const override;

    private:
        std::string metaDataMarker = "introStoryDone";

        std::vector<std::shared_ptr<WorldEntity>> getValidEntities(const EntityMap &entityMap,
                                                                   const WorldModel &worldModel) const;
    };

}
