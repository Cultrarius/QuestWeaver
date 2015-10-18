//
// Created by michael on 10.10.15.
//

#pragma once

#include "../Quest.h"

namespace weave {
    class ExploreRegionQuest : public Quest {
    public:
        ExploreRegionQuest(const std::string &title, const std::string &description);

    private:
        ExploreRegionQuest(ID id,
                           QuestState state,
                           const std::string &title,
                           const std::string &description);

        std::shared_ptr<Quest> setStateAndId(ID newId, QuestState newState) const;

        friend class cereal::access;

        template<class Archive>
        static void load_and_construct(Archive &ar, cereal::construct<ExploreRegionQuest> &construct) {
            ID id;
            QuestState state;
            std::string title;
            std::string description;

            ar(id, state, title, description);
            construct(id, state, title, description);
        }
    };
}

CEREAL_REGISTER_TYPE(weave::ExploreRegionQuest);
