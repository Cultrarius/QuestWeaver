//
// Created by michael on 29.01.16.
//

#include <Story/StoryTemplate.h>

using namespace weave;
using namespace std;

StoryTemplate::StoryTemplate(std::vector<RawStoryLine> rawStoryLines) : rawLines(rawStoryLines) {
}

bool StoryTemplate::IsValid(EntityMap requiredEntities, const WeaverGraph &graph) const { return true; }