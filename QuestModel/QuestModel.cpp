//
// Created by michael on 10.08.15.
//

#include "QuestModel.h"

using namespace std;
using namespace weave;

vector<std::shared_ptr<Quest>> QuestModel::GetActiveQuests() const {
    vector<std::shared_ptr<Quest>> result;
    for (auto &quest : quests) {
        if (quest->GetState() == Active) {
            result.push_back(quest);
        }
    }
    return result;
}

shared_ptr<Quest> QuestModel::RegisterQuest(const Quest &newQuest) {
    // check the quest is not already registered
    for (auto &quest : quests) {
        if (quest->GetId() == newQuest.GetId()) {
            throw ContractFailedException("Quest with id " + to_string(quest->GetId()) + " already registered!");
        }
    }

    idGenerator++;
    shared_ptr<Quest> registeredQuest = newQuest.setStateAndId(idGenerator, QuestState::Inactive);
    quests.push_back(registeredQuest);
    return registeredQuest;
}

std::vector<std::shared_ptr<Quest>> QuestModel::GetQuests() const {
    return quests;
}
