//
// Created by michael on 10.08.15.
//

#pragma once

#include <list>
#include <vector>
#include "Quest.h"

namespace weave {
    class QuestModel {
    public:
        std::list<Quest> getActiveQuests() const;

        void addQuest(Quest newQuest);

    private:
        std::vector<Quest> quests;
    };
}
