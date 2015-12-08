//
// Created by michael on 11.08.15.
//

#pragma once

#include "../Core/WeaverTypes.h"
#include "cereal.h"
#include "../World/WorldModelAction.h"
#include "QuestModelAction.h"

namespace weave {
    enum class QuestState {
        Proposed, Inactive, Active, Failed, Success
    };

    class QuestTickResult {
    public:
        QuestTickResult(std::vector<WorldModelAction> worldChanges, QuestModelAction questChanges);

        std::vector<WorldModelAction> GetWorldChanges() const;

        QuestModelAction GetQuestChange() const;

    private:
        std::vector<WorldModelAction> worldChanges;
        QuestModelAction questChanges;
    };

    class Quest {
    public:
        static const ID NoID = 0;

        Quest(const std::string &title,
              const std::string &description);

        QuestState GetState() const;

        std::string GetTitle() const;

        std::string GetDescription() const;

        ID GetId() const;

        bool operator==(const Quest &other) const;

        virtual std::string GetType() const = 0;

    protected:
        friend class QuestModel;

        friend class QuestWeaver;

        Quest(ID id,
              QuestState state,
              const std::string &title,
              const std::string &description);

        virtual std::shared_ptr<Quest> setStateAndId(ID newId, QuestState newState) const = 0;

        virtual QuestTickResult Tick(float delta);

    protected:
        auto getCerealId() const {
            return cereal::make_nvp("id", id);
        }

        auto getCerealTitle() const {
            return cereal::make_nvp("title", title);
        }

        auto getCerealState() const {
            return cereal::make_nvp("state", state);
        }

        auto getCerealDescription() const {
            return cereal::make_nvp("description", description);
        }

    private:
        ID id;
        QuestState state;
        std::string title;
        std::string description;

        // serialization
        friend class cereal::access;

        template<class Archive>
        void serialize(Archive &archive) {
            archive(getCerealId(), getCerealState(), getCerealTitle(), getCerealDescription());
        }
    };
}
