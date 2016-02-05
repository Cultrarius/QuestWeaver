//
// Created by michael on 29.01.16.
//

#pragma once

#include <Core/WeaverTypes.h>

namespace weave {
    class StoryTemplate {
    public:
        StoryTemplate();

        virtual ~StoryTemplate() = default;

        virtual std::vector<std::string> GetRequiredEntities() const = 0;
    };
}
