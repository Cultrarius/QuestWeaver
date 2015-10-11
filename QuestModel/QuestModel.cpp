//
// Created by michael on 10.08.15.
//

#include "QuestModel.h"

using namespace std;
using namespace weave;

list<Quest> QuestModel::getActiveQuests() const {
    list<Quest> result;
    for (auto &quest : quests) {
        if (quest.getState() == Active) {
            result.push_back(quest);
        }
    }
    return result;
}

void QuestModel::addQuest(Quest newQuest) {
    quests.push_back(newQuest);
}
