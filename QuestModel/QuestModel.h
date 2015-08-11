//
// Created by michael on 10.08.15.
//

#pragma once

#include <list>
#include "Quest.h"

class QuestModel {
public:
    std::list<Quest> getActiveQuests() const;
};
