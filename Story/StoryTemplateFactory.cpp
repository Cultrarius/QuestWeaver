//
// Created by michael on 28.01.16.
//

#include <Story/StoryTemplateFactory.h>

using namespace weave;
using namespace std;
using namespace Json;

vector<shared_ptr<StoryTemplate>> StoryTemplateFactory::GetTemplates() {
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

    for (int i = 0; i < root.size(); i++) {
        Value templateJson = root[i];
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

        templates.push_back(createFromJsonValues(templateJson));
    }
}
