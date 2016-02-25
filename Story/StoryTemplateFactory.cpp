//
// Created by michael on 28.01.16.
//

#include <Story/StoryTemplateFactory.h>

using namespace weave;
using namespace std;
using namespace Json;

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
        throw ContractFailedException(errorMessage);
    }

    for (Value templateJson : root) {
        string requiredMembers[] = {"key", "required", "lines"};
        for (string member : requiredMembers) {
            if (!templateJson.isMember(member)) {
                string errorMessage = "Missing member in template file! MEMBER: <";
                errorMessage += member;
                errorMessage += "> / FILE: ";
                errorMessage += fileName;
                throw ContractFailedException(errorMessage);
            }
        }

        templates[templateJson["key"].asString()] = createFromJsonValues(templateJson);
    }
}

set<string> StoryTemplateFactory::readRequired(const Value &templateJson) const {
    set<string> result;
    for (auto value : templateJson["required"]) {
        result.insert(value.asString());
    }
    return result;
}

vector<RawStoryLine> StoryTemplateFactory::readRawLines(const Json::Value &templateJson) const {
    vector<RawStoryLine> result;
    for (auto value : templateJson["lines"]) {
        RawStoryLine line;

        if (value.isString()) {
            // allow simple strings as story lines
            line.prePart = value.asString();
            result.push_back(move(line));
            continue;
        }

        if (value.isMember("pre")) {
            line.prePart = value["pre"].asString();
        }
        if (value.isMember("post")) {
            line.postPart = value["post"].asString();
        }
        if (value.isMember("nuggets")) {
            for (Value nugget : value["nuggets"]) {
                line.nuggets.insert(nugget.asString());
            }
        }
        result.push_back(move(line));
    }
    return result;
}
