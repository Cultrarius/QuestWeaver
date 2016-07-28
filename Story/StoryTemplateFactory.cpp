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
        Logger::Fatal(errorMessage);
    }

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
