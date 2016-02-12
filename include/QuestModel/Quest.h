//
// Created by michael on 11.08.15.
//

#pragma once

#include "../Core/WeaverTypes.h"
#include "../cereal.h"
#include "../World/WorldModelAction.h"
#include "QuestModelAction.h"

namespace weave {

    /*!
     * Defines the different states a quest can have.
     */
    enum class QuestState {
        /*!
         * Default value used when the quest system does not know the state for a given quest (e.g. unknown ID).
         */
                Unknown,

        /*!
         * The initial state of all newly created quests.
         * The player has not yet started the quest.
         *
         * Quests are only allowed to change their state to Active from this state.
         */
                Inactive,

        /*!
         * The player started the quest and is actively pursuing it.
         * A quest in this state is *not* allowed to go back into the Inactive state.
         */
                Active,

        /*!
         * Represents a quest the player has failed.
         * Final state that cannot be changed any more.
         */
                Failed,

        /*!
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

    /*!
     * Represents a game quest.
     * Instances of this class are immutable.
     * The associated quest state can be changed from within the quest once it is ticked by the quest system.
     */
    class Quest {
    public:
        /*!
         * Quests not registered with the quest system have this id.
         */
        static const ID NoID = 0;

        /*!
         * Initializes a new quest with the given title and description (and an empty story)
         */
        Quest(const std::string &title,
              const std::string &description);

        /*!
         * Initializes a new quest with the given title, description and story
         */
        Quest(const std::string &title,
              const std::string &description,
              const std::string &story);

        virtual ~Quest() { }

        /*!
         * Returns the quest's unique ID.
         */
        ID GetId() const;

        /*!
         * Returns the quest's title.
         */
        std::string GetTitle() const;

        /*!
         * Returns the quest's description.
         */
        std::string GetDescription() const;

        /*!
         * Returns the quest's backstory.
         */
        std::string GetStory() const;

        /*!
         * Returns the type of the quest.
         * This method is implemented by subclasses which return a unique quest type.
         */
        virtual std::string GetType() const = 0;

    protected:
        friend class QuestWeaver;

        Quest(ID id,
              const std::string &title,
              const std::string &description,
              const std::string &story);

        /*!
         * This method is called whenever the main quest system is ticked.
         * It allows subclasses to react to game events and queue changes via the QuestTickResult.
         * @param delta The time elapsed since the last tick.
         */
        virtual QuestTickResult Tick(float delta);

        // serialization helpers

        auto getCerealId() const {
            return cereal::make_nvp("id", id);
        }

        auto getCerealTitle() const {
            return cereal::make_nvp("title", title);
        }

        auto getCerealDescription() const {
            return cereal::make_nvp("description", description);
        }

        auto getCerealStory() const {
            return cereal::make_nvp("story", story);
        }

    private:
        friend class QuestModel;

        ID id;
        std::string title;
        std::string description;
        std::string story;
    };
}
