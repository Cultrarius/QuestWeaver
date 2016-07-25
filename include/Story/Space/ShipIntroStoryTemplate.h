//
// Created by michael on 18.04.16.
//

#pragma once

#include <Story/StoryTemplate.h>

namespace weave {
    class ShipIntroStoryTemplate : public StoryTemplate {
    public:
        explicit ShipIntroStoryTemplate(std::string rawText) : StoryTemplate(rawText, {"spaceShip"}) {}

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