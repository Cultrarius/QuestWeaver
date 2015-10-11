//
// Created by michael on 10.10.15.
//

#pragma once

#include "../Quest.h"

namespace weave {
    class ExploreRegionQuest : Quest {
    public:
        ExploreRegionQuest(QuestState state, const std::string &title, const std::string &description);
    };
}