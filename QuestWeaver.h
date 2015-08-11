//
// Created by michael on 10.08.15.
//

#pragma once

#include <memory>
#include <list>
#include "Core/WeaverEngine.h"
#include "QuestModel/QuestModel.h"
#include "Template/TemplateEngine.h"
#include "World/WorldModel.h"
#include "QuestModel/Quest.h"

class QuestWeaver {
public:
    QuestWeaver();

    std::list<Quest> getActiveQuests() const;
private:
    std::unique_ptr<WeaverEngine> engine;
    std::unique_ptr<QuestModel> quests;
    std::unique_ptr<TemplateEngine> templates;
    std::unique_ptr<WorldModel> world;
};
