//
// Created by michael on 14.02.16.
//

#include "Story/RawStory.h"

uint64_t weave::RawStory::GetLineCount() const noexcept {
    return lines.size();
}

const weave::StoryLine &weave::RawStory::GetLine(uint64_t lineNumber) const {
    return lines.at(lineNumber);
}

weave::RawStory::RawStory(std::vector<weave::StoryLine> lines) : lines(lines) {
}
