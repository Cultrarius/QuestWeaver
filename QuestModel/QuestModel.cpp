//
// Created by michael on 10.08.15.
//

#include <QuestModel/QuestModel.h>

using namespace std;
using namespace weave;

vector<shared_ptr<Quest>> QuestModel::GetQuestsWithState(QuestState state) const {
    vector<shared_ptr<Quest>> result;
    for (auto &quest : quests) {
        if (quest.second->GetState() == state) {
            result.push_back(quest.second);
        }
    }
    return result;
}

shared_ptr<Quest> QuestModel::RegisterNew(shared_ptr<Quest> newQuest,
                                          const vector<QuestPropertyValue> &questProperties) {
    // check the quest is not already registered
    if (quests.find(newQuest->GetId()) != quests.end() || newQuest->GetState() != QuestState::Proposed) {
        throw ContractFailedException("Quest with id " + to_string(newQuest->GetId()) + " already registered!");
    }

    idGenerator++;
    shared_ptr<Quest> registeredQuest = newQuest->setStateAndId(idGenerator, QuestState::Inactive);
    quests[idGenerator] = registeredQuest;
    for (auto property : questProperties) {
        questEntities[idGenerator].insert(property.GetValue());
    }
    return registeredQuest;
}

vector<shared_ptr<Quest>> QuestModel::GetQuests() const {
    vector<shared_ptr<Quest>> result;
    for (auto &quest : quests) {
        result.push_back(quest.second);
    }
    return result;
}

set<shared_ptr<WorldEntity>> QuestModel::GetQuestEntities(ID questId) const {
    auto result = questEntities.find(questId);
    if (result == questEntities.end()) {
        return set<shared_ptr<WorldEntity >>();
    } else {
        return result->second;
    }
}

bool QuestModel::setNewQuestState(ID questId, const QuestState &requiredState, const QuestState &newState) {
    shared_ptr<Quest> quest = GetQuest(questId);
    if (quest->GetState() != requiredState) {
        return false;
    }
    quests[questId] = quest->setStateAndId(questId, newState);
    return true;
}

bool QuestModel::activateQuest(ID questId) {
    return setNewQuestState(questId, QuestState::Inactive, QuestState::Active);
}

bool QuestModel::failQuest(ID questId) {
    return setNewQuestState(questId, QuestState::Active, QuestState::Failed);
}

bool QuestModel::succeedQuest(ID questId) {
    return setNewQuestState(questId, QuestState::Active, QuestState::Success);
}

shared_ptr<Quest> QuestModel::Execute(const QuestModelAction &modelAction) {
    QuestActionType actionType = modelAction.GetActionType();
    ID questId = modelAction.GetQuestId();
    auto iter = quests.find(questId);
    if (actionType == QuestActionType::KEEP && iter == quests.end()) {
        throw ContractFailedException("Quest id " + to_string(questId) + " not found in model!");
    }

    shared_ptr<Quest> result = iter->second;
    if (actionType == QuestActionType::ACTIVATE) {
        if (!activateQuest(questId)) {
            result = nullptr;
        }
    } else if (actionType == QuestActionType::FAIL) {
        if (!failQuest(questId)) {
            result = nullptr;
        }
    } else if (actionType == QuestActionType::SUCCEED) {
        if (!succeedQuest(questId)) {
            result = nullptr;
        }
    } else if (actionType != QuestActionType::KEEP) {
        throw ContractFailedException("Unknown quest model action type");
    }

    actionHistory.push_back(modelAction);
    return result;
}

std::shared_ptr<Quest> QuestModel::GetQuest(ID questId) const {
    auto iter = quests.find(questId);
    if (iter == quests.end()) {
        throw ContractFailedException("Quest with id " + to_string(questId) + " not found in model!");
    }
    return iter->second;
}
