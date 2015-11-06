//
// Created by michael on 10.08.15.
//

#include "QuestModel.h"

using namespace std;
using namespace weave;

vector<std::shared_ptr<Quest>> QuestModel::GetQuestsWithState(QuestState state) const {
    vector<std::shared_ptr<Quest>> result;
    for (auto &quest : quests) {
        if (quest->GetState() == state) {
            result.push_back(quest);
        }
    }
    return result;
}

shared_ptr<Quest> QuestModel::RegisterQuest(const Quest &newQuest,
                                            const std::vector<QuestPropertyValue> &questProperties) {
    // check the quest is not already registered
    for (auto &quest : quests) {
        if (quest->GetId() == newQuest.GetId()) {
            throw ContractFailedException("Quest with id " + to_string(quest->GetId()) + " already registered!");
        }
    }

    idGenerator++;
    shared_ptr<Quest> registeredQuest = newQuest.setStateAndId(idGenerator, QuestState::Inactive);
    quests.push_back(registeredQuest);
    for (auto property : questProperties) {
        questEntities[idGenerator].insert(property.GetValue());
    }
    return registeredQuest;
}

std::vector<std::shared_ptr<Quest>> QuestModel::GetQuests() const {
    return quests;
}

set<shared_ptr<WorldEntity>> QuestModel::GetQuestEntities(ID questId) const {
    auto result = questEntities.find(questId);
    if (result == questEntities.end()) {
        return set<shared_ptr<WorldEntity>>();
    } else {
        return result->second;
    }
}
