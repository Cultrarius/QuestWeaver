//
// Created by michael on 15.08.15.
//

#include "TemplateFactory.h"
#include <iostream>
#include <fstream>

using namespace std;
using namespace Json;

Json::Value TemplateFactory::readTemplateFile(const char *fileName) {
    Value root;
    CharReaderBuilder readBuilder;
    readBuilder["collectComments"] = false;
    readBuilder["rejectDupKeys"] = true;
    string errorMessage;
    ifstream inStream;

    inStream.open(fileName);
    if (!Json::parseFromStream(readBuilder, inStream, &root, &errorMessage)) {
        cerr << "Error parsing template file: " << errorMessage << endl;
        throw new runtime_error(errorMessage);
    } else {
        //TODO check root value for consistency
    }
    return root;
}

vector<string> TemplateFactory::GetTemplateKeys() {
    vector<string> keys;
    keys.reserve(templateMap.size());

    for (auto kv : templateMap) {
        keys.push_back(kv.first);
    }
    return keys;
}

vector<string> TemplateFactory::extractTitles(const Json::Value &root) const {
    vector<string> titles;
    const Value jsonTitles = root["titles"];
    for (int i = 0; i < jsonTitles.size(); ++i) {
        titles.push_back(jsonTitles[i].asString());
    }
    return titles;
}

vector<TemplateQuestDescription> TemplateFactory::extractDescriptions(const Json::Value &root) const {
    vector<TemplateQuestDescription> descriptions;
    const Value jsonDescriptions = root["descriptions"];
    for (int i = 0; i < jsonDescriptions.size(); ++i) {
        const Value jsonDescription = jsonDescriptions[i];
        vector<string> conditions;
        const Value jsonConditions = root["conditions"];
        for (int k = 0; k < jsonConditions.size(); ++k) {
            conditions.push_back(jsonConditions[k].asString());
        }
        TemplateQuestDescription description(conditions, jsonDescription["text"].asString());
        descriptions.push_back(description);
    }
    return descriptions;
}

vector<TemplateQuestProperty> TemplateFactory::extractProperties(const Value &root) const {
    vector<TemplateQuestProperty> properties;
    const Value jsonMandatory = root["mandatory"];
    for (int i = 0; i < jsonMandatory.size(); ++i) {
        string name = jsonMandatory[i].asString();
        TemplateQuestProperty property(true, name);
        properties.push_back(property);
    }

    const Value jsonOptional = root["optional"];
    for (int i = 0; i < jsonOptional.size(); ++i) {
        string name = jsonOptional[i].asString();
        TemplateQuestProperty property(false, name);
        properties.push_back(property);
    }
    return properties;
}

std::shared_ptr<Template> TemplateFactory::CreateTemplate(std::string templateKey) const {
    auto mapEntry = templateMap.find(templateKey);
    if (mapEntry == templateMap.end()) {
        throw new runtime_error("Cannot find template for key " + templateKey + "\n");
    }
    const Value root = mapEntry->second;
    return CreateTemplate(root);
}
