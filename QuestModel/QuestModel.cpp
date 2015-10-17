//
// Created by michael on 10.08.15.
//

#include "QuestModel.h"

using namespace std;
using namespace weave;

list<std::shared_ptr<Quest>> QuestModel::getActiveQuests() const {
    list<std::shared_ptr<Quest>> result;
    for (auto &quest : quests) {
        if (quest->GetState() == Active) {
            result.push_back(quest);
        }
    }
    return result;
}

shared_ptr<Quest> QuestModel::registerQuest(const Quest &newQuest) {
    // TODO check the quest is not already registered
    // TODO create ID
    shared_ptr<Quest> registeredQuest = newQuest.setState(QuestState::Inactive);
    quests.push_back(registeredQuest);
    return registeredQuest;
}
