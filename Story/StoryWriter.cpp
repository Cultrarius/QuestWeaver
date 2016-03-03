//
// Created by michael on 21.12.15.
//

#include <Story/StoryWriter.h>

using namespace std;
using namespace weave;
using namespace Json;

StoryWriter::StoryWriter(shared_ptr<RandomStream> randomStream, const QuestModel &questModel,
                         const TemplateEngine &templateEngine, const WorldModel &worldModel,
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

void StoryWriter::checkValidNuggetJson(Value root, string filePath) const {
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

Story StoryWriter::CreateStory(const WeaverGraph &graph, const vector<QuestPropertyValue> &propertyValues) const {
    // TODO: the current template might also prove useful at this point to pick a more useful story

    initialize();

    unordered_set<string> availableKeys;
    for (const auto &factory : factories) {
        auto templates = factory->GetTemplates();
        for (auto storyTemplate : templates) {
            availableKeys.insert(storyTemplate.first);
        }
    }

    return CreateStory(graph, propertyValues, availableKeys);
}

Story StoryWriter::CreateStory(const WeaverGraph &graph,
                               const vector<QuestPropertyValue> &propertyValues,
                               string storyTemplateKey) const {
    unordered_set<string> keyArray = {storyTemplateKey};
    return CreateStory(graph, propertyValues, keyArray);
}

Story StoryWriter::CreateStory(const WeaverGraph &graph,
                               const vector<QuestPropertyValue> &propertyValues,
                               unordered_set<string> storyTemplateKeys) const {
    Story emptyResult;
    if (graph.GetActiveNodes().empty() || propertyValues.empty()) {
        return emptyResult;
    }
    initialize();

    // find out which templates can be used with the given entities
    auto fittingTemplates = getFittingTemplates(propertyValues, storyTemplateKeys);
    if (fittingTemplates.empty()) {
        return emptyResult;
    }

    // create a property map by ID for faster access
    unordered_map<ID, const QuestPropertyValue *> questValues;
    for (auto &value : propertyValues) {
        questValues[value.GetValue()->GetId()] = &value;
    }

    // create an entity map by type for faster access
    EntityMap entitiesByType;
    for (auto value : propertyValues) {
        entitiesByType[value.GetValue()->GetType()].push_back(value.GetValue());
    }

    map<int, Story> stories = createWeightedStories(graph, fittingTemplates, entitiesByType, questValues);
    if (stories.empty()) {
        return emptyResult;
    }
    return stories.rbegin()->second;
}

vector<shared_ptr<StoryTemplate>> StoryWriter::getFittingTemplates(
        const vector<QuestPropertyValue> &propertyValues, const unordered_set<string> &storyTemplateKeys) const {
    vector<shared_ptr<StoryTemplate>> fittingTemplates;
    for (const auto &factory : this->factories) {
        auto templates = factory->GetTemplates();
        for (auto storyTemplate : templates) {
            if (storyTemplateKeys.count(storyTemplate.first) > 0 &&
                hasAll(storyTemplate.second->GetRequiredEntities(), propertyValues)) {
                fittingTemplates.push_back(storyTemplate.second);
            }
        }
    }
    return fittingTemplates;
}

EntityMap StoryWriter::getPossibleEntitiesForTemplate(
        const shared_ptr<StoryTemplate> &storyTemplate,
        const EntityMap &entitiesByType) const {

    EntityMap requiredEntities;
    for (string required : storyTemplate->GetRequiredEntities()) {
        auto iter = entitiesByType.find(required);
        if (iter != entitiesByType.end()) {
            for (auto entity : (*iter).second) {
                requiredEntities[required].push_back(entity);
            }
        }
    }
    return requiredEntities;
}

bool StoryWriter::hasAll(set<string> requiredEntities, const vector<QuestPropertyValue> &propertyValues) const {
    set<string> availableTypes;
    for (auto questProperty : propertyValues) {
        availableTypes.insert(questProperty.GetValue()->GetType());
    }
    for (string requiredType : requiredEntities) {
        if (availableTypes.count(requiredType) == 0) {
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

map<int, Story> StoryWriter::createWeightedStories(
        const WeaverGraph &graph,
        const vector<shared_ptr<StoryTemplate>> &templates,
        const EntityMap &entitiesByType,
        const unordered_map<ID, const QuestPropertyValue *> &questValues) const {

    map<int, Story> weightedStories;
    for (auto storyTemplate : templates) {
        auto requiredEntities = getPossibleEntitiesForTemplate(storyTemplate, entitiesByType);

        Story currentResult;
        stringstream story;
        int storyValue = 0;
        auto templateResult = storyTemplate->CreateStory(requiredEntities, graph, worldModel);
        currentResult.worldActions = move(templateResult.worldActions);
        for (StoryLine line : templateResult.lines) {
            string prePart = line.GetPrePart();
            string postPart = line.GetPostPart();
            story << prePart;

            auto nuggetOptions = line.GetNuggetOptions();
            if (nuggetOptions.empty()) {
                append(story, prePart, postPart);
                story << " ";
                continue;
            }

            vector<NuggetOption> supportedNuggets = getSupportedNuggets(nuggetOptions, questValues);
            if (supportedNuggets.empty()) {
                // the story is broken at this point, because none of the stories required nuggets are known!
                storyValue = -1;
                break;
            }

            string nuggetText = getRandomNuggetText(questValues, supportedNuggets);
            append(story, prePart, nuggetText);
            append(story, nuggetText, postPart);
            story << " ";
        }
        string storyString = story.str();
        if (!storyString.empty()) {
            // remove trailing whitespace
            storyString.pop_back();
        }
        currentResult.text = storyString;
        weightedStories[storyValue] = currentResult;
    }
    //weightedStories.erase(-1);  // remove broken stories
    auto iter = weightedStories.find(-1);
    if (iter != weightedStories.end()) {
        weightedStories.erase(iter);
    }
    return weightedStories;
}

string StoryWriter::getRandomNuggetText(const unordered_map<ID, const QuestPropertyValue *> &questValues,
                                        const vector<NuggetOption> &supportedNuggets) const {
    NuggetOption chosenOption = supportedNuggets[rs->GetRandomIndex(supportedNuggets.size())];
    Nugget chosenNugget = nuggets[chosenOption.GetNuggetKey()];
    auto texts = chosenNugget.GetTexts();
    string nuggetText = texts[this->rs->GetRandomIndex(texts.size())];
    auto entityTypes = chosenNugget.GetRequiredTypes();
    auto entityIDs = chosenOption.GetEntityIDs();
    if (entityTypes.size() != entityIDs.size()) {
        throw ContractFailedException("Nugget parameter mismatch for key <" + chosenNugget.GetKey() + ">");
    }
    for (uint64_t i = 0; i < entityIDs.size(); i++) {
        ID id = entityIDs[i];
        string from = "%" + entityTypes[i];
        string to = questValues.find(id)->second->GetValueString(this->templateEngine.GetFormat());
        if (!replace(&nuggetText, from, to)) {
            string error("Unable to replace nugget text (i=" + to_string(i) + ", key=" + entityTypes[i] + ")");
            throw ContractFailedException(error);
        }
    }
    return nuggetText;
}

std::vector<NuggetOption> StoryWriter::getSupportedNuggets(
        const vector<NuggetOption> &nuggetOptions,
        const unordered_map<ID, const QuestPropertyValue *> &questValues) const {

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
    return supportedNuggets;
}

void StoryWriter::append(std::stringstream &stream, const string &previous, const string &toAppend) const {
    if (!previous.empty() && !toAppend.empty()) {
        stream << " ";
    }
    stream << toAppend;
}
