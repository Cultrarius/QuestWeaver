//
// Created by michael on 29.01.16.
//

#pragma once

#include <Core/WeaverTypes.h>
#include <Story/RawStory.h>
#include <Core/Graph/WeaverGraph.h>
#include <World/WorldEntity.h>

namespace weave {
    class StoryTemplate {
    public:
        StoryTemplate();

        virtual ~StoryTemplate() = default;

        virtual std::vector<std::string> GetRequiredEntities() const = 0;

        virtual RawStory CreateStory(std::map<std::string, std::shared_ptr<WorldEntity>>,
                                     const WeaverGraph &graph) const = 0;
    };
}
