//
// Created by michael on 15.08.15.
//

#include <Template/QuestTemplateFactory.h>

using namespace std;
using namespace Json;
using namespace weave;

vector<string> QuestTemplateFactory::GetTemplateKeys() {
    initialize();
    vector<string> keys;
    keys.reserve(templateMap.size());

    for (auto kv : templateMap) {
        keys.push_back(kv.first);
    }
    return keys;
}

string QuestTemplateFactory::extractTitle(const Json::Value &root, std::shared_ptr<RandomStream> randomStream) {
    vector<string> titles;
    const Value jsonTitles = root["titles"];
    for (unsigned int i = 0; i < jsonTitles.size(); ++i) {
        titles.push_back(std::move(jsonTitles[i].asString()));
    }
    auto index = randomStream->GetRandomIndex(titles.size());
    return titles.at(index);
}

vector<TemplateQuestDescription> QuestTemplateFactory::extractDescriptions(const Json::Value &root) {
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

vector<TemplateQuestProperty> QuestTemplateFactory::extractProperties(const Value &root) {
    vector<TemplateQuestProperty> properties;
    const Value jsonMandatory = root["mandatory"];
    extractProperties(&properties, jsonMandatory, true);
    const Value jsonOptional = root["optional"];
    extractProperties(&properties, jsonOptional, false);
    return properties;
}

void QuestTemplateFactory::extractProperties(vector<TemplateQuestProperty> *properties, const Value &jsonMandatory,
                                             bool isMandatory) {
    for (unsigned int i = 0; i < jsonMandatory.size(); ++i) {
        string name = jsonMandatory[i].asString();
        TemplateQuestProperty property(isMandatory, name);
        properties->push_back(std::move(property));
    }
}

std::shared_ptr<QuestTemplate> QuestTemplateFactory::CreateTemplate(const std::string &templateKey) {
    initialize();
    auto mapEntry = templateMap.find(templateKey);
    if (mapEntry == templateMap.end()) {
        throw ContractFailedException("Cannot find template for key " + templateKey + "\n");
    }
    const Value root = mapEntry->second;
    return createFromJsonValues(root);
}

void QuestTemplateFactory::initialize() {
    if (isInitialized) {
        return;
    }
    isInitialized = true;
    templateMap.clear();

    const char *fileName = getTemplateFile();
    Value root = readJsonFromFile(fileName, dirs);

    // quick sanity check
    string requiredMembers[] = {"key", "parent", "mandatory", "optional", "titles", "descriptions", "objectives"};
    for (string member : requiredMembers) {
        if (!root.isMember(member)) {
            string errorMessage = "Missing member in template file! MEMBER: <";
            errorMessage += member;
            errorMessage += "> / FILE: ";
            errorMessage += fileName;
            throw ContractFailedException(errorMessage);
        }
    }

    templateMap[root["key"].asString()] = root;
}
