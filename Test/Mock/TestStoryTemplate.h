//
// Created by michael on 05.02.16.
//

#pragma once

#include <Story/StoryTemplate.h>

namespace weave {
    class TestStoryTemplate : public StoryTemplate {
    public:
        bool ReturnInvalidIDs = false;

        TestStoryTemplate(std::set<std::string> requiredTypes, std::string rawText,
                          std::vector<WorldModelAction> actions)
                : StoryTemplate(rawText, requiredTypes), actions(actions) {
        }

        StoryTemplateResult CreateStory(const EntityMap &entities, const WeaverGraph &,
                                        const WorldModel &,
                                        std::shared_ptr<RandomStream>) const override {
            TokenToEntityMap tokenEntityMap;
            for (auto token : getStoryTokens(rawText)) {
                if (token.id.empty()) {
                    continue;
                }
                if (!entities.empty() && !GetRequiredEntities().empty()) {
                    std::string someType = *GetRequiredEntities().begin();
                    if (ReturnInvalidIDs) {
                        tokenEntityMap[token.id].push_back(133337);
                    } else {
                        ID id = entities.find(someType)->second.at(0)->GetId();
                        tokenEntityMap[token.id].push_back(id);
                    }
                }
            }

            StoryTemplateResult result;
            result.rawText = rawText;
            result.worldActions = actions;
            result.tokenMap = createTokenMapping(tokenEntityMap);
            return result;
        }

    private:
        std::vector<WorldModelAction> actions;
    };
}
