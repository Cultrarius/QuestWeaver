//
// Created by michael on 15.08.15.
//

#include "TemplateFactory.h"

using namespace std;
using namespace Json;
using namespace weave;

Json::Value TemplateFactory::readTemplateFile(const char *fileName) {
    Value root;
    CharReaderBuilder readBuilder;
    readBuilder["collectComments"] = false;
    readBuilder["rejectDupKeys"] = true;
    string errorMessage;
    ifstream inStream;
    openFile(fileName, &inStream);
    if (!Json::parseFromStream(readBuilder, inStream, &root, &errorMessage)) {
        cerr << "Error parsing template file: " << errorMessage << endl;
        throw errorMessage;
    } else {
        // TODO check root value for consistency
    }
    return root;
}

vector<string> TemplateFactory::GetTemplateKeys() {
    initialize();
    vector<string> keys;
    keys.reserve(templateMap.size());

    for (auto kv : templateMap) {
        keys.push_back(kv.first);
    }
    return keys;
}

string TemplateFactory::extractTitle(const Json::Value &root) const {
    vector<string> titles;
    const Value jsonTitles = root["titles"];
    for (unsigned int i = 0; i < jsonTitles.size(); ++i) {
        titles.push_back(std::move(jsonTitles[i].asString()));
    }
    auto index = randomStream->GetRandomIndex(titles.size());
    return titles.at(index);
}

vector<TemplateQuestDescription> TemplateFactory::extractDescriptions(const Json::Value &root) const {
    vector<TemplateQuestDescription> descriptions;
    const Value jsonDescriptions = root["descriptions"];
    for (unsigned int i = 0; i < jsonDescriptions.size(); ++i) {
        const Value jsonDescription = jsonDescriptions[i];
        vector<string> conditions;
        const Value jsonConditions = jsonDescription["conditions"];
        for (unsigned int k = 0; k < jsonConditions.size(); ++k) {
            conditions.push_back(std::move(jsonConditions[k].asString()));
        }
        TemplateQuestDescription description(conditions, jsonDescription["text"].asString());
        descriptions.push_back(std::move(description));
    }
    return descriptions;
}

vector<TemplateQuestProperty> TemplateFactory::extractProperties(const Value &root) const {
    vector<TemplateQuestProperty> properties;
    const Value jsonMandatory = root["mandatory"];
    extractProperties(&properties, jsonMandatory, true);
    const Value jsonOptional = root["optional"];
    extractProperties(&properties, jsonOptional, false);
    return properties;
}

void TemplateFactory::extractProperties(vector<TemplateQuestProperty> *properties, const Value &jsonMandatory,
                                        bool isMandatory) const {
    for (unsigned int i = 0; i < jsonMandatory.size(); ++i) {
        string name = jsonMandatory[i].asString();
        TemplateQuestProperty property(isMandatory, name);
        properties->push_back(std::move(property));
    }
}

std::shared_ptr<Template> TemplateFactory::CreateTemplate(const std::string &templateKey) {
    this->randomStream = randomStream;
    initialize();
    auto mapEntry = templateMap.find(templateKey);
    if (mapEntry == templateMap.end()) {
        throw ContractFailedException("Cannot find template for key " + templateKey + "\n");
    }
    const Value root = mapEntry->second;
    return createFromJsonValues(root);
}

void TemplateFactory::openFile(const char *fileName, ifstream *inStream) {
    string modDir(dirs.modDirectory);
    string dataDir(dirs.templateDirectory);

    // try to use the mods directory
    const char *moddedFile = modDir.append(fileName).c_str();
    inStream->open(moddedFile);
    if (!inStream->fail()) {
        return;
    }

    // try to use the regular directory
    const char *templateFile = dataDir.append(fileName).c_str();
    inStream->open(templateFile);
    if (!inStream->fail()) {
        return;
    }

    // try to open the file as it is
    inStream->open(fileName);
    if (!inStream->fail()) {
        return;
    }
    std::string errorMsg =
            "Unable to find file in any of the following directories: [., " + dirs.templateDirectory + ", " +
            dirs.modDirectory +
            "]";
    throw ContractFailedException(errorMsg);
}

void TemplateFactory::initialize() {
    if (isInitialized) {
        return;
    }
    isInitialized = true;
    templateMap.clear();

    const char *fileName = getTemplateFile();
    auto root = readTemplateFile(fileName);
    if (root["parent"].asString() != "Space") {
        string errorMessage = string("Template file has incompatible parent: ") + fileName + "\n";
        cerr << errorMessage;
        throw ContractFailedException(errorMessage);
    }

    templateMap[root["key"].asString()] = root;
}
