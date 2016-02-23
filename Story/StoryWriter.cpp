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
    for (const auto &factory : factories) {
        factory->initialize();
    }
    readNuggets();
}

void StoryWriter::readNuggets() const {
    nuggets.clear();
    set<string> nuggetFolders;
    for (const auto &factory : factories) {
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
    for (const auto &factory : factories) {
        for (auto storyTemplate : factory->GetTemplates()) {
            if (hasAll(storyTemplate->GetRequiredEntities(), propertyValues)) {
                fittingTemplates.push_back(storyTemplate);
            }
        }
    }

    if (fittingTemplates.empty()) {
        return "";
    }

    unordered_map<ID, QuestPropertyValue *> questValues;
    for (auto value : propertyValues) {
        questValues[value.GetValue()->GetId()] = &value;
    }

    for (auto storyTemplate : fittingTemplates) {
        map<string, shared_ptr<WorldEntity>> requiredEntities;
        for (string required : storyTemplate->GetRequiredEntities()) {
            for (auto value : propertyValues) {
                if (value.GetValue()->GetType() == required) {
                    requiredEntities[required] = value.GetValue();
                    break;
                }
            }
        }

        stringstream story;
        for (StoryLine line : storyTemplate->CreateStory(requiredEntities, graph)) {
            story << line.GetPrePart();

            auto nuggetOptions = line.GetNuggetOptions();
            if (nuggetOptions.empty()) {
                story << line.GetPostPart();
                continue;
            }

            vector<NuggetOption> supportedNuggets;
            for (NuggetOption option : nuggetOptions) {
                for (ID entityId : option.GetEntityIDs()) {
                    if (questValues.count(entityId) == 0) {
                        // the template knows which IDs are allowed, so this should never happen
                        throw ContractFailedException("Invalid nugget option (ID " + to_string(entityId) + ")");
                    }
                }
                if (nuggets.count(option.GetNuggetKey()) == 0) {
                    // the template does not know which nuggets are actually registered, so this can happen
                    continue;
                }
                supportedNuggets.push_back(option);
            }
            NuggetOption chosenOption = supportedNuggets[rs->GetRandomIndex(supportedNuggets.size())];
            Nugget chosenNugget = nuggets[chosenOption.GetNuggetKey()];
            auto texts = chosenNugget.GetTexts();
            string nuggetText = texts[rs->GetRandomIndex(texts.size())];
            auto entityTypes = chosenNugget.GetRequiredTypes();
            auto entityIDs = chosenOption.GetEntityIDs();
            if (entityTypes.size() != entityIDs.size()) {
                throw ContractFailedException("Nugget parameter mismatch for key <" + chosenNugget.GetKey() + ">");
            }
            for (uint64_t i = 0; i < entityIDs.size(); i++) {
                string from = "%" + entityTypes[i];
                string to = questValues[entityIDs[i]]->GetValueString(templateEngine.GetFormat());
                if (!replace(&nuggetText, from, to)) {
                    string error("Unable to replace nugget text (i=" + to_string(i) + ", key=" + entityTypes[i] + ")");
                    throw ContractFailedException(error);
                }
            }
            story << nuggetText << line.GetPostPart();

            //TODO gather stories choose between them
            return story.str();
        }
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
    for (const auto &factory : factories) {
        factory->dirs = dirs;
        factory->isInitialized = false;
    }
}

void StoryWriter::RegisterTemplateFactory(unique_ptr<StoryTemplateFactory> factory) {
    factory->dirs = dirs;
    factories.push_back(move(factory));
}
