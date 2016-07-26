//
// Created by michael on 10.08.15.
//

#include <QuestModel/QuestModel.h>

using namespace std;
using namespace weave;

vector<shared_ptr<Quest>> QuestModel::GetQuestsWithState(QuestState state) const noexcept {
    vector<shared_ptr<Quest>> result;
    if (state == QuestState::Unknown) {
        return result;
    }
    for (auto &quest : quests) {
        ID id = quest.second->GetId();
        if (GetState(id) == state) {
            result.push_back(quest.second);
        }
    }
    return result;
}

void QuestModel::RegisterNew(shared_ptr<Quest> newQuest,
                             const vector<QuestPropertyValue> &questProperties,
                             const std::string &storyText) {
    // check the quest is not already registered
    ID id = newQuest->GetId();
    if (quests.count(id) + questStates.count(id) > 0) {
        Logger::Error(ContractFailedException("Quest id " + to_string(newQuest->GetId()) + " already registered!"));
        return;
    }

    idGenerator++;
    newQuest->id = idGenerator;
    newQuest->story = storyText;
    quests[idGenerator] = newQuest;
    questStates[idGenerator] = QuestState::Inactive;
    for (auto property : questProperties) {
        questEntities[idGenerator].insert(property.GetValue());
    }
}

vector<shared_ptr<Quest>> QuestModel::GetQuests() const noexcept {
    vector<shared_ptr<Quest>> result;
    for (auto quest : quests) {
        result.push_back(quest.second);
    }
    return result;
}

set<shared_ptr<WorldEntity>> QuestModel::GetQuestEntities(ID questId) const noexcept {
    auto result = questEntities.find(questId);
    if (result == questEntities.end()) {
        return set<shared_ptr<WorldEntity >>();
    } else {
        return result->second;
    }
}

bool QuestModel::setNewQuestState(ID questId, const QuestState &requiredState, const QuestState &newState) {
    if (GetState(questId) != requiredState) {
        return false;
    }
    questStates[questId] = newState;
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

bool QuestModel::Execute(const QuestModelAction &modelAction) {
    QuestActionType actionType = modelAction.GetActionType();
    ID questId = modelAction.GetQuestId();
    if (actionType == QuestActionType::KEEP && quests.count(questId) == 0) {
        Logger::Error(ContractFailedException("Quest id " + to_string(questId) + " not found in the model!"));
        return false;
    }

    bool result = true;
    if (actionType == QuestActionType::ACTIVATE) {
        if (!activateQuest(questId)) {
            result = false;
        }
    } else if (actionType == QuestActionType::FAIL) {
        if (!failQuest(questId)) {
            result = false;
        }
    } else if (actionType == QuestActionType::SUCCEED) {
        if (!succeedQuest(questId)) {
            result = false;
        }
    } else if (actionType != QuestActionType::KEEP) {
        Logger::Error(ContractFailedException("Unknown quest model action type"));
        return false;
    }

    actionHistory.push_back(modelAction);
    return result;
}

shared_ptr<Quest> QuestModel::GetQuest(ID questId) const {
    auto iter = quests.find(questId);
    if (iter == quests.end()) {
        auto ex = ContractFailedException("Quest with id " + to_string(questId) + " not found in the model!");
        Logger::Fatal(ex);
        throw ex;
    }
    return iter->second;
}

QuestState QuestModel::GetState(ID questId) const noexcept {
    auto iter = questStates.find(questId);
    if (iter == questStates.end()) {
        return QuestState::Unknown;
    }
    return iter->second;
}
