//
// Created by michael on 21.11.15.
//

#include <QuestModel/QuestModelAction.h>

using namespace weave;
using namespace std;

QuestActionType QuestModelAction::GetActionType() const {
    return actionType;
}

ID QuestModelAction::GetQuestId() const {
    return questId;
}
