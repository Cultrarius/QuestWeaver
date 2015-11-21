//
// Created by michael on 21.11.15.
//

#pragma once

#include <memory>
#include "cereal.h"
#include "Quest.h"
#include "../Template/Template.h"

namespace weave {

    enum class QuestActionType {
        KEEP, REGISTER, ACTIVATE, FAIL, SUCCEED
    };

    class QuestModelAction {
    public:
        QuestModelAction() :
                actionType(QuestActionType::KEEP) { }

        QuestModelAction(const QuestActionType &actionType,
                         const std::shared_ptr<Quest> quest) :
                actionType(actionType), quest(quest) { }

        QuestModelAction(const QuestActionType &actionType,
                         const std::shared_ptr<Quest> quest,
                         const std::vector<QuestPropertyValue> &questProperties) :
                actionType(actionType), quest(quest), questProperties(questProperties) { }

        QuestActionType GetActionType() const;

        std::shared_ptr<Quest> GetQuest() const;

        std::vector<QuestPropertyValue> GetProperties() const;

    private:
        QuestActionType actionType;
        std::shared_ptr<Quest> quest;
        std::vector<QuestPropertyValue> questProperties;

        // serialization
        friend class cereal::access;

        template<class Archive>
        void serialize(Archive &archive) {
            archive(CEREAL_NVP(actionType), CEREAL_NVP(quest), CEREAL_NVP(questProperties));
        }
    };
}

