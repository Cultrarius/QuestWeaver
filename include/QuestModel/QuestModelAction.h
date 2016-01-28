//
// Created by michael on 21.11.15.
//

#pragma once

#include <memory>
#include "cereal.h"
#include "../Core/WeaverTypes.h"

namespace weave {

    enum class QuestActionType {
        KEEP, ACTIVATE, FAIL, SUCCEED
    };

    class QuestModelAction {
    public:
        QuestModelAction() :
                actionType(QuestActionType::KEEP) { }

        QuestModelAction(const QuestActionType &actionType, ID questId) :
                actionType(actionType), questId(questId) { }

        QuestActionType GetActionType() const;

        ID GetQuestId() const;

    private:
        QuestActionType actionType;
        ID questId;

        // serialization
        friend class cereal::access;

        template<class Archive>
        void serialize(Archive &archive) {
            archive(CEREAL_NVP(actionType), CEREAL_NVP(questId));
        }
    };
}
