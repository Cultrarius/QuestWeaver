//
// Created by michael on 21.12.15.
//

#include <Story/StoryWriter.h>

using namespace std;
using namespace weave;
using namespace Json;

StoryWriter::StoryWriter(shared_ptr<RandomStream> randomStream, const QuestModel &questModel,
                         const TemplateEngine &templateEngine, const weave::WorldModel &worldModel,
                         const Directories &dirs) :
        rs(randomStream), questModel(questModel), templateEngine(templateEngine), worldModel(worldModel), dirs(dirs) {
}

void StoryWriter::initialize() const {
    if (isInitialized) {
        return;
    }
    isInitialized = true;
    for (auto factory : factories) {
        factory->initialize();
    }
    readNuggets();
}

void StoryWriter::readNuggets() const {
    nuggets.clear();
    set<string> nuggetFolders;
    for (auto factory : factories) {
        nuggetFolders.insert(factory->GetNuggetFolder());
    }

    for (string folder : nuggetFolders) {
        string file = folder;
        file += "/Nuggets.st";
        Value root = readJsonFromFile(file.c_str(), dirs);
        checkValidNuggetJson(root, file);
        for (Value nuggetJson : root) {
            string key = nuggetJson["key"].asString();
            vector<string> requiredTypes;
            for (Value requiredType : nuggetJson["requiredTypes"]) {
                requiredTypes.push_back(requiredType.asString());
            }
            vector<string> texts;
            for (Value text : nuggetJson["texts"]) {
                texts.push_back(text.asString());
            }
            if (nuggets.count(key) > 0) {
                throw ContractFailedException("Duplicate nugget key <" + key + ">!");
            }
            nuggets[key] = Nugget(key, requiredTypes, texts);
        }
    }
}

void StoryWriter::checkValidNuggetJson(Json::Value root, std::string filePath) const {
    if (!root.isArray()) {
        throw ContractFailedException("Invalid JSON in nugget file " + filePath + " (no root array)");
    }
    for (Value nuggetJson : root) {
        if (!nuggetJson.isObject()) {
            throw ContractFailedException("Invalid JSON in nugget file " + filePath + " (no nugget object)");
        }
        string requiredMembers[] = {"key", "requiredTypes", "texts"};
        for (string member : requiredMembers) {
            if (!nuggetJson.isMember(member)) {
                string errorMessage = "Missing member in nugget file! MEMBER: <";
                errorMessage += member;
                errorMessage += "> / FILE: ";
                errorMessage += filePath;
                throw ContractFailedException(errorMessage);
            }
        }
    }
}

string StoryWriter::CreateStory(const WeaverGraph &graph, const vector<QuestPropertyValue> &propertyValues) const {
    // TODO: the current template might also prove useful at this point to pick a more useful story

    initialize();

    if (graph.GetActiveNodes().empty() || propertyValues.empty()) {
        return "";
    }

    vector<shared_ptr<StoryTemplate>> fittingTemplates;
    for (auto factory : factories) {
        for (auto storyTemplate : factory->GetTemplates()) {
            if (hasAll(storyTemplate->GetRequiredEntities(), propertyValues)) {
                fittingTemplates.push_back(storyTemplate);
            }
        }
    }

    for (auto storyTemplate : fittingTemplates) {
        map<string, shared_ptr<WorldEntity>> requiredEntities;
        for (string required : storyTemplate->GetRequiredEntities()) {
            for (auto value : propertyValues) {
                if (value.GetValue()->GetType() == required) {
                    requiredEntities[required] = value.GetValue();
                }
            }
        }
        RawStory rawResult = storyTemplate->CreateStory(requiredEntities, graph);
    }

    if (fittingTemplates.empty()) {
        return "";
    }

    return "In a far away galaxy a long time ago...\nThere were three little piglets!";
}

bool StoryWriter::hasAll(vector<string> requiredEntities, const std::vector<QuestPropertyValue> &propertyValues) const {
    for (string required : requiredEntities) {
        bool hasRequired = false;
        for (auto questProperty : propertyValues) {
            if (required == questProperty.GetValue()->GetType()) {
                hasRequired = true;
                break;
            }
        }
        if (!hasRequired) {
            return false;
        }
    }
    return true;
}

void StoryWriter::ChangeDirectories(const Directories &newDirs) {
    dirs = newDirs;
    isInitialized = false;
    for (auto factory : factories) {
        factory->dirs = dirs;
        factory->isInitialized = false;
    }
}

void StoryWriter::RegisterTemplateFactory(shared_ptr<StoryTemplateFactory> factory) {
    factory->dirs = dirs;
    factories.push_back(factory);
}
