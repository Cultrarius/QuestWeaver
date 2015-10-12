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
    templateMap.clear();

    const char *fileName = "../Template/Space/ExploreRegionTemplate.qt";
    auto root = readTemplateFile(fileName);
    if (root["parent"].asString().compare("Space") != 0) {
        string errorMessage = string("Template file has incompatible parent: ") + fileName + "\n";
        cerr << errorMessage;
        throw new runtime_error(errorMessage);
    }

    templateMap[root["key"].asString()] = root;
}

std::shared_ptr<Template> SpaceQuestTemplateFactory::CreateTemplate(const Json::Value &root) const {
    string title = extractTitle(root);
    vector<TemplateQuestProperty> properties = extractProperties(root);
    vector<TemplateQuestDescription> descriptions = extractDescriptions(root);

    const string &templateKey = root["key"].asString();
    if (templateKey.compare("ExploreRegionQuest") == 0) {
        auto exploreTemplate = make_shared<ExploreRegionTemplate>(title, properties, descriptions);

        return exploreTemplate;
    } else {
        throw new runtime_error("Unknown Space template key " + templateKey + "\n");
    }
}
