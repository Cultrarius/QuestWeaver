//
// Created by michael on 28.01.16.
//

#include <Story/StoryTemplateFactory.h>
#include <Story/SimpleStoryTemplate.h>

using namespace weave;
using namespace std;
using namespace Json;
using namespace std::regex_constants;

map<string, shared_ptr<StoryTemplate>> StoryTemplateFactory::GetTemplates() {
    initialize();
    return templates;
}

void StoryTemplateFactory::initialize() {
    if (isInitialized) {
        return;
    }
    isInitialized = true;
    templates.clear();

    const char *fileName = getTemplateFile();
    Value root = readJsonFromFile(fileName, dirs);

    // quick sanity check
    if (!root.isArray()) {
        string errorMessage = "Invalid Story template file, expected Array as root! FILE: ";
        errorMessage += fileName;
        Logger::Fatal(errorMessage);
    }

    map<StoryCondition, regex> conditionRegexes = {
            {StoryCondition::OncePerEntity, regex("once per entity", icase | optimize)},
            {StoryCondition::WithoutProperty, regex("does not have (\\w+)", icase | optimize)}
    };

    for (Value templateJson : root) {
        string requiredMembers[] = {"key", "required", "text"};
        for (string member : requiredMembers) {
            if (!templateJson.isMember(member)) {
                string errorMessage = "Missing member in template file! MEMBER: <";
                errorMessage += member;
                errorMessage += "> / FILE: ";
                errorMessage += fileName;
                Logger::Fatal(errorMessage);
            }
        }

        string key = templateJson["key"].asString();
        string text = templateJson["text"].asString();
        string type = templateJson.get("type", Value("simple")).asString();

        ConditionMap conditions;
        for (Value rawCondition : templateJson.get("conditions", Value(arrayValue))) {
            string condition = rawCondition.asString();
            for (auto pair : conditionRegexes) {
                smatch matchResult;
                if (regex_match(condition, matchResult, pair.second)) {
                    vector<string> properties;
                    for (uint64_t i = 1; i < matchResult.size(); i++) {
                        std::ssub_match base_sub_match = matchResult[i];
                        properties.push_back(base_sub_match.str());
                    }
                    conditions[pair.first] = properties;
                    break;
                }
            }
        }

        set<string> requiredTypes;
        for (Value required : templateJson["required"]) {
            requiredTypes.insert(required.asString());
        }

        if (type == "simple") {
            templates[key] = make_shared<SimpleStoryTemplate>(key, text, requiredTypes, conditions);
        } else if (type == "complex") {
            templates[key] = createFromJsonValues(templateJson, key, text, requiredTypes, conditions);
        } else {
            Logger::Error("Unknown optional type '" + type + "' for story template with key '" + key +
                          "'. Allowed types: 'simple' or 'complex'. (File " + fileName + ").");
        }
    }
}
