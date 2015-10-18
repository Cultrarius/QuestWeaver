//
// Created by michael on 11.08.15.
//

#pragma once

#include "../Core/WeaverTypes.h"
#include "cereal.h"

namespace weave {
    enum QuestState {
        Proposed, Inactive, Active, Failed, Success
    };

    class Quest {
    public:
        const ID NoID = 0;

        Quest(const std::string &title,
              const std::string &description);

        QuestState GetState() const;

        std::string GetTitle() const;

        std::string GetDescription() const;

        ID GetId() const;

    protected:
        friend class QuestModel;

        Quest(ID id,
              QuestState state,
              const std::string &title,
              const std::string &description);

        virtual std::shared_ptr<Quest> setStateAndId(ID newId, QuestState newState) const = 0;


    private:
        ID id;
        QuestState state;
        std::string title;
        std::string description;

        // serialization
        friend class cereal::access;

        template<class Archive>
        void serialize(Archive &archive) {
            archive(CEREAL_NVP(id), CEREAL_NVP(state), CEREAL_NVP(title), CEREAL_NVP(description));
        }

        template<class Archive>
        static void load_and_construct(Archive &ar, cereal::construct<Quest> &construct) {
            ID id;
            QuestState state;
            std::string title;
            std::string description;

            ar(id, state, title, description);
            construct(id, state, title, description);
        }
    };
}
