//
// Created by michael on 21.11.15.
//

#include "QuestModelAction.h"

using namespace weave;
using namespace std;

QuestActionType QuestModelAction::GetActionType() const {
    return actionType;
}

shared_ptr<Quest> QuestModelAction::GetQuest() const {
    return quest;
}

vector<QuestPropertyValue> QuestModelAction::GetProperties() const {
    return questProperties;
}
