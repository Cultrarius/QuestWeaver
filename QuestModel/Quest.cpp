//
// Created by michael on 11.08.15.
//

#include "Quest.h"

using namespace weave;
using namespace std;

QuestState Quest::GetState() const {
    return state;
}

string Quest::GetTitle() const {
    return title;
}

string Quest::GetDescription() const {
    return description;
}

Quest::Quest(const string &title, const string &description) :
        Quest(NoID, QuestState::Proposed, title, description) {
}

Quest::Quest(ID newId, QuestState state, const string &title, const string &description) :
        id(newId), state(state), title(title), description(description) {
}


ID Quest::GetId() const {
    return id;
}

QuestTickResult Quest::Tick(float delta) {
    vector<WorldModelAction> emptyChanges;
    QuestModelAction emptyAction(QuestActionType::KEEP, id);
    return QuestTickResult(emptyChanges, emptyAction);
}

bool Quest::operator==(const Quest &other) const {
    return id == other.id;
}

QuestTickResult::QuestTickResult(vector<WorldModelAction> worldChanges, QuestModelAction questChanges) :
        worldChanges(worldChanges), questChanges(questChanges) {
}

vector<WorldModelAction> QuestTickResult::GetWorldChanges() const {
    return worldChanges;
}

QuestModelAction QuestTickResult::GetQuestChange() const {
    return questChanges;
}
