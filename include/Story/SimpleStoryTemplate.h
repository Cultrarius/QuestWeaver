//
// Created by michael on 05.02.16.
//

#pragma once

#include <Story/StoryTemplate.h>

namespace weave {
    class SimpleStoryTemplate : public StoryTemplate {
    public:
        SimpleStoryTemplate(std::string key, std::string rawText, std::set<std::string> requiredTypes,
                            ConditionMap conditions) :
                StoryTemplate(rawText, requiredTypes), key(key), conditions(conditions) {}

        StoryTemplateResult CreateStory(const EntityMap &requiredEntities, const WeaverGraph &graph,
                                        const WorldModel &worldModel,
                                        std::shared_ptr<RandomStream> randomStream) const override;

        bool IsValid(const EntityMap &requiredEntities, const WeaverGraph &graph,
                     const WorldModel &worldModel) const override;

    private:
        std::string key;
        ConditionMap conditions;

        std::vector<std::shared_ptr<WorldEntity>> getValidEntities(const EntityMap &entityMap,
                                                                   const WorldModel &worldModel,
                                                                   std::shared_ptr<RandomStream> randomStream) const;
    };

}
