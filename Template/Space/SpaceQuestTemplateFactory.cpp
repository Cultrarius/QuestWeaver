//
// Created by michael on 15.08.15.
//

#include "SpaceQuestTemplateFactory.h"
#include "ExploreRegionTemplate.h"
#include <iostream>

using namespace std;
using namespace Json;

SpaceQuestTemplateFactory::SpaceQuestTemplateFactory() {
    templateMap.clear();

    const char *fileName = "ExploreRegionTemplate.qt";
    auto root = readTemplateFile(fileName);
    if (root["parent"].asString().compare("Space") != 0) {
        string errorMessage = string("Template file has incompatible parent: ") + fileName + "\n";
        cerr << errorMessage;
        throw new runtime_error(errorMessage);
    }

//    const Json::Value descriptions = root["descriptions"];
//    for (int i = 0; i < descriptions.size(); ++i) {
//        const Json::Value description = descriptions[i];
//        cout << description["text"].asString() << endl;
//    }
    templateMap[root["key"].asString()] = root;
}

std::shared_ptr<Template> SpaceQuestTemplateFactory::CreateTemplate(std::string templateKey) {
    auto mapEntry = templateMap.find(templateKey);
    if (mapEntry == templateMap.end()) {
        throw new runtime_error("Cannot find template for key " + templateKey + "\n");
    }
    std::vector<std::string> titles;
    std::vector<TemplateQuestProperty> properties;
    std::vector<TemplateQuestDescription> descriptions;
    if (mapEntry->first.compare("ExploreRegionQuest") == 0) {
        auto exploreTemplate = make_shared<ExploreRegionTemplate>(titles, properties, descriptions);

        return exploreTemplate;
    } else {
        throw new runtime_error("Unknown Space template key " + templateKey + "\n");
    }
}
