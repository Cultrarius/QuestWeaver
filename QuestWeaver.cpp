//
// Created by michael on 10.08.15.
//

#include "QuestWeaver.h"
#include <algorithm>
#include <iostream>
#include "Template/Space/SpaceQuestTemplateFactory.h"

using namespace std;
using namespace weave;

QuestWeaver::QuestWeaver() {
    engine.reset(new WeaverEngine());
    quests.reset(new QuestModel());
    templates.reset(new TemplateEngine());
    world.reset(new WorldModel());

    shared_ptr<TemplateFactory> spaceFactory = make_shared<SpaceQuestTemplateFactory>();
    templates->RegisterTemplateFactory(spaceFactory);
}

std::list<Quest> QuestWeaver::GetActiveQuests() const {
    return quests->getActiveQuests();
}

Quest QuestWeaver::CreateNewQuest() {
    auto questTemplate = templates->GetTemplateForNewQuest();
    vector<QuestPropertyValue> questPropertyValues = engine->fillTemplate(questTemplate, *world);
    Quest newQuest = questTemplate->ToQuest(questPropertyValues);
    // TODO create quest-variants and choose the best one
    updateWorld(newQuest);
    return newQuest;
}

void QuestWeaver::updateWorld(Quest quest) {
}
