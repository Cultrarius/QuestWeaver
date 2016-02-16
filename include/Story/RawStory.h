//
// Created by michael on 14.02.16.
//

#pragma once

#include <Core/WeaverTypes.h>
#include "Nugget.h"

namespace weave {

    struct StoryLine {
        std::string prePart;
        std::vector<std::string> nuggetKeys;
        std::string postPart;
    };

    class RawStory {
    public:
        explicit RawStory(std::vector<StoryLine> lines);

        uint64_t GetLineCount() const noexcept;

        const StoryLine &GetLine(uint64_t lineNumber) const;

    private:
        std::vector<StoryLine> lines;
    };

}
