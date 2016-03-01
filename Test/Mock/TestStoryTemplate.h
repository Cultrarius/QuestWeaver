//
// Created by michael on 05.02.16.
//

#pragma once

#include <Story/StoryTemplate.h>

namespace weave {
    class TestStoryTemplate : public StoryTemplate {
    public:
        bool ReturnInvalidIDs = false;

        TestStoryTemplate(std::set<std::string> requiredTypes, std::vector<RawStoryLine> rawLines,
                          std::vector<WorldModelAction> actions)
                : StoryTemplate(rawLines), requiredTypes(requiredTypes), actions(actions) {
        }

        std::set<std::string> GetRequiredEntities() const override {
            return requiredTypes;
        }

        StoryTemplateResult CreateStory(std::map<std::string, std::vector<std::shared_ptr<WorldEntity>>> entities,
                                        const WeaverGraph &graph) const override {
            std::vector<StoryLine> lines;

            for (auto rawLine : rawLines) {
                std::vector<NuggetOption> options;
                for (auto nuggetKey : rawLine.nuggets) {
                    std::vector<ID> ids;
                    if (!entities.empty() && !requiredTypes.empty()) {
                        std::string someType = *requiredTypes.begin();
                        if (ReturnInvalidIDs) {
                            ids.push_back(133337);
                        } else {
                            ids.push_back(entities[someType][0]->GetId());
                        }
                    }
                    options.push_back(NuggetOption(nuggetKey, ids));
                }
                lines.push_back(StoryLine(rawLine.prePart, options, rawLine.postPart));
            }

            StoryTemplateResult result;
            result.lines = move(lines);
            result.worldActions = actions;
            return result;
        }

    private:
        std::set<std::string> requiredTypes;
        std::vector<WorldModelAction> actions;
    };
}
