//
// Created by michael on 10.08.15.
//

#pragma once

#include <memory>
#include "Core/WeaverEngine.h"

class QuestWeaver {
public:
    QuestWeaver();

private:
    std::unique_ptr<WeaverEngine> engine;
};
