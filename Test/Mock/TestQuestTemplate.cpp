//
// Created by michael on 09.02.16.
//

#include "TestQuestTemplate.h"
#include "TestQuest.h"

using namespace weave;
using namespace std;

TestQuestTemplate::TestQuestTemplate(vector<TemplateQuestProperty> properties,
                                     vector<TemplateQuestDescription> descriptions)
        : QuestTemplate("", properties, descriptions, FormatterType::TEXT) {
}

shared_ptr<Quest> TestQuestTemplate::ToQuest(const vector<QuestPropertyValue> &questPropertyValues,
                                             const string &questStory) const {
    string description = getBestFittingDescription(questPropertyValues);
    string title = getTitle(questPropertyValues);
    return make_shared<TestQuest>(title, description);
}

vector<WorldModelAction> TestQuestTemplate::GetPropertyCandidates(const TemplateQuestProperty &property,
                                                                  const WorldModel &worldModel) const {
    return vector<WorldModelAction>();
}
