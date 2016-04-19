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
    for (auto &value : propertyValues) {
        questValues[value.GetValue()->GetId()] = &value;
    }

    EntityMap entitiesByType;
    for (auto value : propertyValues) {
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

            if (!token.isMandatory && rs->GetIntInRange(0, 1) == 1) {
                replace(&story, token.text, "");
                continue;
            }

            auto ids = pair.second;
            vector<NuggetOption> nuggetOptions;
            for (string option : token.nuggetOptions) {
                nuggetOptions.push_back(NuggetOption(option, ids));
            }

            vector<NuggetOption> supportedNuggets = getSupportedNuggets(nuggetOptions, questValues);
            storyValue += nuggetWeight * supportedNuggets.size();
            if (supportedNuggets.empty() && token.isMandatory) {
                // the story is broken at this point, because none of the required nuggets are known!
                storyValue = -1;
                break;
            }

            string nuggetText = getRandomNuggetText(questValues, supportedNuggets);
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

string StoryWriter::getRandomNuggetText(const QuestValueMap &questValues,
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
        string requiredType = entityTypes[i];
        auto questProperty = questValues.find(id)->second;
        string actualType = questProperty->GetValue()->GetType();
        if (requiredType != actualType) {
            string error("Invalid types <" + requiredType + "> and <" + actualType + ">");
            throw ContractFailedException(error);
        }
        string from = "%" + requiredType;
        string to = questProperty->GetValueString(this->templateEngine.GetFormat());
        if (!replace(&nuggetText, from, to)) {
            string error("Unable to replace nugget text (i=" + to_string(i) + ", key=" + requiredType + ")");
            throw ContractFailedException(error);
        }
    }
    return nuggetText;
}

std::vector<NuggetOption> StoryWriter::getSupportedNuggets(const vector<NuggetOption> &nuggetOptions,
                                                           const QuestValueMap &questValues) const {
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
