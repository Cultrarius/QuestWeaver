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
    };
}
