//
// Created by michael on 21.12.15.
//

#include <Story/StoryWriter.h>

using namespace std;
using namespace weave;
using namespace Json;

Nugget::Nugget(std::string key, std::vector<std::string> requiredTypes, std::vector<std::string> texts) :
        key(key), requiredTypes(requiredTypes), texts(texts) {
}

std::string Nugget::GetKey() {
    return key;
}

std::vector<std::string> Nugget::GetRequiredTypes() {
    return requiredTypes;
}

std::vector<std::string> Nugget::GetTexts() {
    return texts;
}

StoryWriter::StoryWriter(shared_ptr<RandomStream> randomStream, const QuestModel &model,
                         const TemplateEngine &templateEngine, const Directories &dirs) :
        rs(randomStream), model(model), templateEngine(templateEngine), dirs(dirs) {
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
            if (nuggets.find(key) != nuggets.end()) {
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

string StoryWriter::CreateStory(const WeaverGraph &graph) const {
    // TODO: the current template might also prove useful at this point to pick a more useful story

    initialize();

    for (auto factory : factories) {
        for (auto storyTemplate : factory->GetTemplates()) {
            //TODO create story
        }
    }

    return "In a far away galaxy a long time ago...\nThere were three little piglets!";
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
    factories.push_back(factory);
}
