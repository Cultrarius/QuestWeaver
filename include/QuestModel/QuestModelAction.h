//
// Created by michael on 21.11.15.
//

#pragma once

#include <memory>
#include "cereal.h"
#include "../Core/WeaverTypes.h"

namespace weave {

    /*!
     * Describes the action the quest model should execute for a quest.
     */
    enum class QuestActionType {
        //
                KEEP, /*!< Keep a quest in the model */
                ACTIVATE, /*!< Activate an inactive quest */
                FAIL, /*!< Set an active quest to the failed state */
                SUCCEED /*!< Set an active quest to the succeeded state */
    };

    /*!
     * These actions are used by the quest system to change the state of quests.
     */
    class QuestModelAction {
    public:

        /*!
         * Creates a new default action (which are pretty much useless).
         */
        QuestModelAction() :
                actionType(QuestActionType::KEEP), questId(0) { }

        /*!
         * Creates a new action with the given type and quest ID.
         */
        QuestModelAction(const QuestActionType &actionType, ID questId) :
                actionType(actionType), questId(questId) { }

        /*!
         * Returns the action that should be used on the quest state.
         */
        QuestActionType GetActionType() const;

        /*!
         * Returns the quest ID this action is valid for.
         */
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
