//
// Created by michael on 01.02.16.
//

#include <Story/Space/CommonSpaceStoryFactory.h>

using namespace std;
using namespace weave;

shared_ptr<StoryTemplate> CommonSpaceStoryFactory::createFromJsonValues(const Json::Value &root) const {
    return std::shared_ptr<StoryTemplate>();
}

const char *CommonSpaceStoryFactory::getTemplateFile() const {
    return "Space/CommonSpaceStories.st";
}
