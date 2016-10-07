//
// Created by michael on 01.02.16.
//

#include <Story/Space/CommonSpaceStoryFactory.h>
#include <Story/Space/ShipIntroStoryTemplate.h>

using namespace std;
using namespace weave;
using namespace Json;

shared_ptr<StoryTemplate>
CommonSpaceStoryFactory::createFromJsonValues(const Value &, string key, string text,
                                              set<string>, set<StoryCondition>) const {
    if (key == "shipIntro") {
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
