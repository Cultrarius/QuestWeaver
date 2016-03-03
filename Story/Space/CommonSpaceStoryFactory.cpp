//
// Created by michael on 01.02.16.
//

#include <Story/Space/CommonSpaceStoryFactory.h>
#include "Story/Space/AgentIntroStoryTemplate.h"

using namespace std;
using namespace weave;

shared_ptr<StoryTemplate> CommonSpaceStoryFactory::createFromJsonValues(const Json::Value &root) const {
    string key = root["key"].asString();
    if (key == "agentIntro") {
        return make_shared<AgentIntroStoryTemplate>(readRawLines(root));
    } else {
        throw ContractFailedException("Unknown story template key [" + key +
                                      "], maybe you forgot to create a corresponding StoryTemplate class?");
    }
}

const char *CommonSpaceStoryFactory::getTemplateFile() const {
    return "Space/CommonSpaceStories.st";
}