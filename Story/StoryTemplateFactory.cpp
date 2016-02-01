//
// Created by michael on 28.01.16.
//

#include <Story/StoryTemplateFactory.h>

using namespace weave;
using namespace std;
using namespace Json;

vector<shared_ptr<StoryTemplate>> StoryTemplateFactory::GetTemplates() {
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

    /*
    // quick sanity check
    string requiredMembers[] = {"key", "parent", "mandatory", "optional", "titles", "descriptions", "objectives"};
    for (string member : requiredMembers) {
        if (!root.isMember(member)) {
            string errorMessage = "Missing member in template file! MEMBER: <";
            errorMessage += member;
            errorMessage += "> / FILE: ";
            errorMessage += fileName;
            throw ContractFailedException(errorMessage);
        }
    }
     */

    templates.push_back(createFromJsonValues(root));
}
