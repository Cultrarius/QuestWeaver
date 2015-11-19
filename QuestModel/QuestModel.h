//
// Created by michael on 10.08.15.
//

#pragma once

#include "Quest.h"
#include "../Template/Template.h"

namespace weave {
    class QuestModel {
    public:
        std::vector<std::shared_ptr<Quest>> GetQuestsWithState(QuestState state) const;

        std::vector<std::shared_ptr<Quest>> GetQuests() const;

        std::shared_ptr<Quest> RegisterQuest(const Quest &newQuest,
                                             const std::vector<QuestPropertyValue> &questProperties);

        std::set<std::shared_ptr<WorldEntity>> GetQuestEntities(ID questId) const;

        bool ActivateQuest(ID questId);

        bool FailQuest(ID questId);

        bool SucceedQuest(ID questId);

    private:
        std::vector<std::shared_ptr<Quest>> quests;

        std::map<ID, std::set<std::shared_ptr<WorldEntity>>> questEntities;

        ID idGenerator = 0;

        // serialization
        friend class cereal::access;

        template<class Archive>
        void serialize(Archive &archive) {
            archive(CEREAL_NVP(idGenerator), CEREAL_NVP(quests), CEREAL_NVP(questEntities));
        }

        bool setNewQuestState(ID questId, const QuestState &requiredState, const QuestState &newState);
    };
}
