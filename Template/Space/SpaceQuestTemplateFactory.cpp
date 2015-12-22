//
// Created by michael on 15.08.15.
//

#include "SpaceQuestTemplateFactory.h"
#include "ExploreRegionTemplate.h"
#include <iostream>

using namespace std;
using namespace Json;
using namespace weave;

SpaceQuestTemplateFactory::SpaceQuestTemplateFactory(std::shared_ptr<RandomStream> randomStream)
        : TemplateFactory(randomStream) {
}

SpaceQuestTemplateFactory::SpaceQuestTemplateFactory(std::shared_ptr<RandomStream> randomStream,
                                                     Directories directories) :
        TemplateFactory(randomStream, directories) {
}

std::shared_ptr<Template> SpaceQuestTemplateFactory::createFromJsonValues(const Json::Value &root) const {
    string title = extractTitle(root);
    vector<TemplateQuestProperty> properties = extractProperties(root);
    vector<TemplateQuestDescription> descriptions = extractDescriptions(root);

    const string &templateKey = root["key"].asString();
    if (templateKey == "ExploreRegionQuest") {
        auto exploreTemplate = make_shared<ExploreRegionTemplate>(title, properties, descriptions);

        return exploreTemplate;
    } else {
        throw ContractFailedException("Unknown Space template key " + templateKey + "\n");
    }
}

const char *SpaceQuestTemplateFactory::getTemplateFile() const {
    return "Space/ExploreRegionTemplate.qt";
}


