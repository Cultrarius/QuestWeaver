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
            if (nuggets.count(key) > 0) {
                Logger::Fatal(ContractFailedException("Duplicate nugget key <" + key + ">!"));
            }

            vector<string> requiredTypes;
            for (Value requiredType : nuggetJson["requiredTypes"]) {
                requiredTypes.push_back(requiredType.asString());
            }

            vector<string> texts;
            for (Value text : nuggetJson["texts"]) {
                texts.push_back(text.asString());
            }

            unordered_map<string, int> minValues;
            unordered_map<string, int> maxValues;
            readRandomizedValues(nuggetJson, minValues, maxValues);

            unordered_map<string, NameType> randomNames;
            readRandomizedNames(nuggetJson, randomNames);

            float rarity = 0;
            if (nuggetJson.isMember("rarity")) {
                rarity = nuggetJson["rarity"].asFloat();
                if (rarity < 0) {
                    Logger::Error(ContractFailedException("Rarity for nugget key <" + key + "> is smaller than 0!"));
                    rarity = 0;
                }
            }

            nuggets[key] = Nugget(key, rarity, requiredTypes, texts, minValues, maxValues, randomNames);
        }
    }
}

void StoryWriter::readRandomizedNames(const Value &nuggetJson, unordered_map<string, NameType> &randomNames) const {
    if (!nuggetJson.isMember("randomizedName")) {
        return;
    }
    for (Value randomization : nuggetJson["randomizedName"]) {
        string randomKey = randomization["key"].asString();
        NameType type = NameType::DARK_THING;
        if (randomization.isMember("type")) {
            string typeString = randomization["type"].asString();
            if (typeString == "dark_thing") {
                type = NameType::DARK_THING;
            } else if (typeString == "dark_person") {
                type = NameType::DARK_PERSON;
            } else if (typeString == "alien") {
                type = NameType::ALIEN;
            } else if (typeString == "light_thing") {
                type = NameType::LIGHT_THING;
            } else if (typeString == "light_person") {
                type = NameType::LIGHT_PERSON;
            } else if (typeString == "corporation") {
                type = NameType::CORPORATION;
            } else if (typeString == "funny") {
                type = NameType::FUNNY;
            } else if (typeString == "ship") {
                type = NameType::SHIP;
            } else if (typeString == "cuddly") {
                type = NameType::CUDDLY;
            } else if (typeString == "cuddly_verb") {
                type = NameType::CUDDLY_VERB;
            } else {
                Logger::Error(ContractFailedException("Unknown name type <" + typeString + ">!"));
            }
        }
        randomNames[randomKey] = type;
    }
}

void StoryWriter::readRandomizedValues(const Value &nuggetJson, unordered_map<string, int> &minValues,
                                       unordered_map<string, int> &maxValues) const {
    if (!nuggetJson.isMember("randomizedValue")) {
        return;
    }
    for (Value randomization : nuggetJson["randomizedValue"]) {
        string randomKey = randomization["key"].asString();
        if (randomization.isMember("min")) {
            minValues[randomKey] = randomization["min"].asInt();
        }
        if (randomization.isMember("max")) {
            maxValues[randomKey] = randomization["max"].asInt();
        }
    }
}

void StoryWriter::checkValidNuggetJson(Value root, string filePath) const {
    if (!root.isArray()) {
        Logger::Fatal(ContractFailedException("Invalid JSON in nugget file " + filePath + " (no root array)"));
    }
    for (Value nuggetJson : root) {
        if (!nuggetJson.isObject()) {
            Logger::Fatal(ContractFailedException("Invalid JSON in nugget file " + filePath + " (no nugget object)"));
        }
        string requiredMembers[] = {"key", "requiredTypes", "texts"};
        for (string member : requiredMembers) {
            if (!nuggetJson.isMember(member)) {
                string errorMessage = "Missing member in nugget file! MEMBER: <";
                errorMessage += member;
                errorMessage += "> / FILE: ";
                errorMessage += filePath;
                Logger::Fatal(ContractFailedException(errorMessage));
            }
        }
    }
}

Story StoryWriter::CreateStory(const StoryWriterParameters &params) const {
    initialize();

    unordered_set<string> availableKeys;
    for (const auto &factory : factories) {
        auto templates = factory->GetTemplates();
        for (auto storyTemplate : templates) {
            availableKeys.insert(storyTemplate.first);
        }
    }

    return CreateStory(params, availableKeys);
}

Story StoryWriter::CreateStory(const StoryWriterParameters &params,
                               string storyTemplateKey) const {
    unordered_set<string> keyArray = {storyTemplateKey};
    return CreateStory(params, keyArray);
}

Story StoryWriter::CreateStory(const StoryWriterParameters &params,
                               unordered_set<string> storyTemplateKeys) const {
    Story emptyResult;
    auto graph = params.graph;
    auto propertyValues = params.propertyValues;
    if (graph.GetActiveNodes().empty() || propertyValues.empty()) {
        return emptyResult;
    }
    initialize();

    // find out which templates can be used with the given entities
    auto fittingTemplates = getFittingTemplates(propertyValues, storyTemplateKeys);
    if (fittingTemplates.empty()) {
        return emptyResult;
    }

    QuestValueMap questValues;
    EntityMap entitiesByType;
    for (auto &value : propertyValues) {
        questValues[value.GetValue()->GetId()] = &value;
        entitiesByType[value.GetValue()->GetType()].push_back(value.GetValue());
    }

    map<float, Story> stories = createWeightedStories(graph, fittingTemplates, entitiesByType, questValues);
    removeStoriesWithInvalidActions(params, &stories);

    if (stories.empty()) {
        return emptyResult;
    }
    return stories.rbegin()->second;  // map is sorted
}

/*
 * It is possible that a story wants to change the world model in an invalid way. This happens when a quest wants to
 * delete an entity from the world model while the story wants to update or keep it. Since the stories are not vital
 * and to avoid inconsistencies, they are just deleted when that happens.
 */
void StoryWriter::removeStoriesWithInvalidActions(const StoryWriterParameters &params,
                                                  std::map<float, Story> *storyMap) const {
    unordered_set<ID> deletedEntities;
    for (auto action : params.modelActions) {
        if (action.GetActionType() == WorldActionType::DELETE) {
            deletedEntities.insert(action.GetEntity()->GetId());
        }
    }
    vector<float> storiesWithInvalidActions;
    for (auto pair : *storyMap) {
        for (auto action : pair.second.worldActions) {
            if (deletedEntities.count(action.GetEntity()->GetId()) > 0) {
                storiesWithInvalidActions.push_back(pair.first);
            }
        }
    }
    for (float key : storiesWithInvalidActions) {
        auto iter = storyMap->find(key);
        if (iter != storyMap->end()) {
            storyMap->erase(iter);
        }
    }
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

EntityMap StoryWriter::getPossibleEntitiesForTemplate(const shared_ptr<StoryTemplate> &storyTemplate,
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

map<float, Story> StoryWriter::createWeightedStories(
        const WeaverGraph &graph,
        const vector<shared_ptr<StoryTemplate>> &templates,
        const EntityMap &entitiesByType,
        const QuestValueMap &questValues) const {

    map<float, Story> weightedStories;
    for (auto storyTemplate : templates) {
        auto requiredEntities = getPossibleEntitiesForTemplate(storyTemplate, entitiesByType);
        if (!storyTemplate->IsValid(requiredEntities, graph, worldModel)) {
            continue;
        }

        Story currentResult;

        float storyValue = storyEntityWeight * storyTemplate->GetRequiredEntities().size();
        auto templateResult = storyTemplate->CreateStory(requiredEntities, graph, worldModel);
        currentResult.worldActions = move(templateResult.worldActions);
        storyValue += worldActionWeight * currentResult.worldActions.size();

        string story = templateResult.rawText;
        for (auto pair : templateResult.tokenMap) {
            RawStoryToken token = pair.first;

            auto ids = pair.second;
            vector<NuggetOption> nuggetOptions;
            float minRarity = 0;
            for (string option : token.nuggetOptions) {
                nuggetOptions.push_back(NuggetOption(option, ids));
                auto nuggetIter = nuggets.find(option);
                if (nuggetIter != nuggets.end()) {
                    minRarity = min(minRarity, nuggetIter->second.GetRarity());
                }
            }

            // sort out optional tokens by rarity
            int rarityToReach = static_cast<int>(minRarity + 1);
            if (!token.isMandatory && rs->GetIntInRange(0, rarityToReach) == rarityToReach) {
                replace(&story, token.text, "");
                continue;
            }

            vector<NuggetOption> supportedNuggets = getSupportedNuggets(nuggetOptions);
            storyValue += nuggetWeight * supportedNuggets.size();
            if (supportedNuggets.empty() && token.isMandatory) {
                // the story is broken at this point, because none of the required nuggets are known!
                storyValue = -1;
                break;
            }

            uint64_t nuggetIndex = getNuggetIndexByRarity(supportedNuggets);
            NuggetOption chosenOption = supportedNuggets[nuggetIndex];

            // check previous quests for nugget entity matches
            unordered_set<ID> nuggetIDs;
            for (ID id : chosenOption.GetEntityIDs()) {
                nuggetIDs.insert(id);
            }
            for (auto quest : questModel.GetQuests()) {
                auto questEntities = questModel.GetQuestEntities(quest->GetId());
                QuestState state = questModel.GetState(quest->GetId());
                for (auto entity : questEntities) {
                    if (nuggetIDs.count(entity->GetId())) {
                        if (state == QuestState::Success || state == QuestState::Failed) {
                            storyValue += finishedQuestWeight;
                        } else {
                            storyValue += unfinishedQuestWeight;
                        }
                    }
                }
            }

            // place the nugget in the story
            string nuggetText = getNuggetText(questValues, chosenOption);
            replace(&story, token.text, nuggetText);
        }

        if (templateEngine.GetFormat() == FormatterType::HTML) {
            replaceAll(&story, "\n", "<br/>\n");
            currentResult.text = htmlEncloseWithTag(story, "span", "story");
        } else {
            currentResult.text = story;
        }
        if (storyValue >= 0) {
            storyValue += storyCharWeight * story.length();
            // turn the weight into a probability
            storyValue = storyValue * (rs->GetIntInRange(0, 90) / 90.0f + 0.1f);
        }
        weightedStories[storyValue] = currentResult;
    }
    // remove broken stories
    auto iter = weightedStories.find(-1);
    if (iter != weightedStories.end()) {
        weightedStories.erase(iter);
    }
    return weightedStories;
}

string StoryWriter::getNuggetText(const QuestValueMap &questValues, const NuggetOption &chosenOption) const {
    Nugget chosenNugget = nuggets[chosenOption.GetNuggetKey()];
    auto texts = chosenNugget.GetTexts();
    string nuggetText = texts[this->rs->GetRandomIndex(texts.size())];

    for (string key : chosenNugget.GetRandomizationKeys()) {
        string from = "%" + key;
        string to = chosenNugget.GetRandomValue(key, rs, worldModel.GetNameGenerator());
        replaceAll(&nuggetText, from, to);
    }

    auto entityTypes = chosenNugget.GetRequiredTypes();
    auto entityIDs = chosenOption.GetEntityIDs();
    if (entityTypes.size() != entityIDs.size()) {
        Logger::Fatal(ContractFailedException("Nugget parameter mismatch for key <" + chosenNugget.GetKey() + ">"));
    }
    for (uint64_t i = 0; i < entityIDs.size(); i++) {
        ID id = entityIDs[i];
        string requiredType = entityTypes[i];
        QuestPropertyValue questProperty = getQuestValueForID(questValues, id);
        string actualType = questProperty.GetValue()->GetType();
        if (requiredType != actualType) {
            string error("Invalid types <" + requiredType + "> and <" + actualType + ">");
            Logger::Fatal(ContractFailedException(error));
        }
        string from = "%" + requiredType;
        string to = questProperty.GetValueString(this->templateEngine.GetFormat());
        if (!replace(&nuggetText, from, to)) {
            string error("Unable to replace nugget text (i=" + to_string(i) + ", key=" + requiredType + ")");
            Logger::Error(ContractFailedException(error));
        }
    }

    return nuggetText;
}

QuestPropertyValue StoryWriter::getQuestValueForID(const QuestValueMap &questValues, ID id) const {
    auto iter = questValues.find(id);
    if (iter != questValues.end()) {
        return *iter->second;
    }
    // the story template used an entity not from the quest values, so we make a temp quest object to help
    // with the formatting
    return QuestPropertyValue(TemplateQuestProperty(false, "tmp"), worldModel.GetEntityById(id));
}

uint64_t StoryWriter::getNuggetIndexByRarity(const vector<NuggetOption> &supportedNuggets) const {
    map<float, uint64_t> nuggetProbability;
    for (uint64_t i = 0; i < supportedNuggets.size(); i++) {
        auto nugget = nuggets[supportedNuggets[i].GetNuggetKey()];
        float probability = 1 - rs->GetIntInRange(0, 100) / (100 * (nugget.GetRarity() + 1));
        nuggetProbability[probability] = i;
    }
    return (*nuggetProbability.begin()).second;
}

std::vector<NuggetOption> StoryWriter::getSupportedNuggets(const vector<NuggetOption> &nuggetOptions) const {
    vector<NuggetOption> supportedNuggets;
    for (NuggetOption option : nuggetOptions) {
        for (ID entityId : option.GetEntityIDs()) {
            if (!worldModel.GetEntityById(entityId)) {
                // the template knows which IDs are allowed, so this should never happen
                Logger::Fatal(ContractFailedException("Invalid nugget option (ID " + to_string(entityId) + ")"));
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
