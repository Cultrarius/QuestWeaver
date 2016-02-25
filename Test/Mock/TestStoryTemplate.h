//
// Created by michael on 05.02.16.
//

#pragma once

#include <Story/StoryTemplate.h>

namespace weave {
    class TestStoryTemplate : public StoryTemplate {
    public:
        TestStoryTemplate(std::vector<std::string> requiredTypes, std::vector<RawStoryLine> rawLines)
                : StoryTemplate(rawLines) {
            this->requiredTypes = requiredTypes;
        }

        std::vector<std::string> GetRequiredEntities() const override {
            return requiredTypes;
        }

        std::vector<StoryLine> CreateStory(std::map<std::string, std::vector<std::shared_ptr<WorldEntity>>> entities,
                                           const WeaverGraph &graph) const override {
            std::vector<StoryLine> lines;

            for (auto rawLine : rawLines) {
                std::vector<NuggetOption> options;
                for (auto nuggetKey : rawLine.nuggets) {
                    std::vector<ID> ids;
                    if (!entities.empty() && !requiredTypes.empty()) {
                        ids.push_back(entities[requiredTypes[0]][0]->GetId());
                    }
                    options.push_back(NuggetOption(nuggetKey, ids));
                }
                lines.push_back(StoryLine(rawLine.prePart, options, rawLine.postPart));
            }

            return lines;
        }


    private:
        std::vector<std::string> requiredTypes;
    };
}
