//
// Created by michael on 01.02.16.
//

#include <Story/Space/CommonSpaceStoryFactory.h>
#include <Story/Space/LocationIntroStoryTemplate.h>
#include <Story/Space/ShipIntroStoryTemplate.h>
#include "Story/Space/AgentIntroStoryTemplate.h"
#include "Story/Space/PlanetIntroStoryTemplate.h"

using namespace std;
using namespace weave;

shared_ptr<StoryTemplate> CommonSpaceStoryFactory::createFromJsonValues(const Json::Value &root) const {
    string key = root["key"].asString();
    string text = root["text"].asString();
    if (key == "agentIntro") {
        return make_shared<AgentIntroStoryTemplate>(text);
    } else if (key == "locationIntro") {
        return make_shared<LocationIntroStoryTemplate>(text);
    } else if (key == "planetIntro") {
        return make_shared<PlanetIntroStoryTemplate>(text);
    } else if (key == "shipIntro") {
        return make_shared<ShipIntroStoryTemplate>(text);
    } else {
        ContractFailedException ex("Unknown story template key [" + key +
                                   "], maybe you forgot to create a corresponding StoryTemplate class?");
        Logger::Fatal(ex);
        throw ex;
    }
}

const char *CommonSpaceStoryFactory::getTemplateFile() const {
    return "Space/CommonSpaceStories.st";
}
