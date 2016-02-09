//
// Created by michael on 11.08.15.
//

#pragma once

#include "../Core/WeaverTypes.h"
#include "../cereal.h"
#include "../World/WorldModelAction.h"
#include "QuestModelAction.h"

namespace weave {

    /*
     * Defines the different states a quest can have.
     */
    enum class QuestState {
        /*
         * Used only internally for quest candidates that the system uses when creating a new quest.
         */
                Proposed,

        /*
         * The initial state of all newly created quests.
         * The player has not yet started the quest.
         *
         * Quests are only allowed to change their state to Active from this state.
         */
                Inactive,

        /*
         * The player started the quest and is actively pursuing it.
         * A quest in this state is *not* allowed to go back into the Inactive state.
         */
                Active,

        /*
         * Represents a quest the player has failed.
         * Final state that cannot be changed any more.
         */
                Failed,

        /*
         * Represents a quest the player has successfully completed.
         * Final state that cannot be changed any more.
         */
                Success
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

        Quest(const std::string &title,
              const std::string &description,
              const std::string &story);

		virtual ~Quest() {}

        QuestState GetState() const;

        std::string GetTitle() const;

        std::string GetDescription() const;

        std::string GetStory() const;

        ID GetId() const;

        bool operator==(const Quest &other) const;

        virtual std::string GetType() const = 0;


    protected:
        friend class QuestModel;

        friend class QuestWeaver;

        Quest(ID id,
              QuestState state,
              const std::string &title,
              const std::string &description,
              const std::string &story);

        virtual std::shared_ptr<Quest> setStateAndId(ID newId, QuestState newState) const = 0;

        virtual QuestTickResult Tick(float delta);

        // serialization helpers

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

        auto getCerealStory() const {
            return cereal::make_nvp("story", story);
        }

    private:
        ID id;
        QuestState state;
        std::string title;
        std::string description;
        std::string story;
    };
}
