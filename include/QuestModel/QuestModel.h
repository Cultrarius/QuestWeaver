//
// Created by michael on 10.08.15.
//

#pragma once

#include <QuestModel/Quest.h>
#include <Template/QuestTemplate.h>
#include <QuestModel/QuestModelAction.h>

namespace weave {
    class QuestModel {
    public:
        std::vector<std::shared_ptr<Quest>> GetQuestsWithState(QuestState state) const;

        std::vector<std::shared_ptr<Quest>> GetQuests() const;

        std::shared_ptr<Quest> GetQuest(ID questId) const;

        QuestState GetState(ID questId) const noexcept;

        std::set<std::shared_ptr<WorldEntity>> GetQuestEntities(ID questId) const;

        std::shared_ptr<Quest> Execute(const QuestModelAction &modelAction);

        void RegisterNew(std::shared_ptr<Quest> newQuest,
                         const std::vector<QuestPropertyValue> &questProperties);

    private:
        std::map<ID, std::shared_ptr<Quest>> quests;

        std::unordered_map<ID, QuestState> questStates;

        std::map<ID, std::set<std::shared_ptr<WorldEntity>>> questEntities;

        std::vector<QuestModelAction> actionHistory;

        ID idGenerator = 0;

        bool activateQuest(ID questId);

        bool failQuest(ID questId);

        bool succeedQuest(ID questId);

        // serialization
        friend class cereal::access;

        template<class Archive>
        void serialize(Archive &archive) {
            archive(CEREAL_NVP(idGenerator), CEREAL_NVP(quests), CEREAL_NVP(questStates), CEREAL_NVP(questEntities));
        }

        bool setNewQuestState(ID questId, const QuestState &requiredState, const QuestState &newState);
    };
}
