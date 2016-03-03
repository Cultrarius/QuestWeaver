//
// Created by michael on 29.01.16.
//

#pragma once

#include <Core/WeaverTypes.h>
#include <Story/StoryLine.h>
#include <Core/Graph/WeaverGraph.h>
#include <World/WorldEntity.h>
#include <World/WorldModelAction.h>
#include <World/WorldModel.h>

namespace weave {

    typedef std::unordered_map<std::string, std::vector<std::shared_ptr<WorldEntity>>> EntityMap;

    /*!
     * @ingroup storyApi
     */
    struct RawStoryLine {
        std::string prePart;
        std::string postPart;
        std::set<std::string> nuggets;
    };

    /*!
     * @ingroup storyApi
     */
    struct StoryTemplateResult {
        std::vector<StoryLine> lines;
        std::vector<WorldModelAction> worldActions;
    };

    /*!
     * @ingroup storyApi
     */
    class StoryTemplate {
    public:
        explicit StoryTemplate(std::vector<RawStoryLine> rawStoryLines);

        virtual ~StoryTemplate() = default;

        virtual std::set<std::string> GetRequiredEntities() const = 0;

        virtual bool IsValid(const EntityMap &requiredEntities, const WeaverGraph &graph,
                             const WorldModel &worldModel) const;

        virtual StoryTemplateResult CreateStory(const EntityMap &requiredEntities, const WeaverGraph &graph,
                                                const WorldModel &worldModel) const = 0;

    protected:
        std::vector<RawStoryLine> rawLines;
    };
}
