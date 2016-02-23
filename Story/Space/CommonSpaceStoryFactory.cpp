//
// Created by michael on 01.02.16.
//

#include <Story/Space/CommonSpaceStoryFactory.h>
#include "Story/Space/AgentIntroStoryTemplate.h"

using namespace std;
using namespace weave;

shared_ptr<StoryTemplate> CommonSpaceStoryFactory::createFromJsonValues(const Json::Value &root) const {
    return make_shared<AgentIntroStoryTemplate>(readRawLines(root));
}

const char *CommonSpaceStoryFactory::getTemplateFile() const {
    return "Space/CommonSpaceStories.st";
}