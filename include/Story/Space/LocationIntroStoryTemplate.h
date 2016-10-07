//
// Created by michael on 12.03.16.
//

#pragma once

#include <Story/StoryTemplate.h>

namespace weave {
    class LocationIntroStoryTemplate : public StoryTemplate {
    public:
        explicit LocationIntroStoryTemplate(std::string rawText) : StoryTemplate(rawText, {"solarSystem"}) { }

        StoryTemplateResult CreateStory(const EntityMap &requiredEntities, const WeaverGraph &graph,
                                        const WorldModel &worldModel,
                                        std::shared_ptr<RandomStream> randomStream) const override;

        bool IsValid(const EntityMap &requiredEntities, const WeaverGraph &graph,
                     const WorldModel &worldModel) const override;

    private:
        std::string metaDataMarker = "introStoryDone";

        std::vector<std::shared_ptr<WorldEntity>> getValidEntities(const EntityMap &entityMap,
                                                                   const WorldModel &worldModel) const;
    };

}
