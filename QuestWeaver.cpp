//
// Created by michael on 10.08.15.
//

#include "QuestWeaver.h"
#include <algorithm>
#include <iostream>
#include "Template/Space/SpaceQuestTemplateFactory.h"
#include "World/Space/SpaceWorldModel.h"

using namespace std;
using namespace weave;

QuestWeaver::QuestWeaver(uint64_t seed) {
    randomStream.reset(new RandomStream(seed));
    engine.reset(new WeaverEngine(randomStream));
    quests.reset(new QuestModel());
    templates.reset(new TemplateEngine());
    world.reset(new SpaceWorldModel(randomStream));

    shared_ptr<TemplateFactory> spaceFactory = make_shared<SpaceQuestTemplateFactory>(randomStream);
    templates->RegisterTemplateFactory(spaceFactory);
}

shared_ptr<Quest> QuestWeaver::CreateNewQuest() {
    auto questTemplate = templates->GetTemplateForNewQuest(randomStream);
    EngineResult result = engine->fillTemplate(questTemplate, *quests, *world);
    world->Execute(result.GetModelActions());
    shared_ptr<Quest> newQuest = questTemplate->ToQuest(result.GetQuestPropertyValues());
    return quests->RegisterNew(newQuest, result.GetQuestPropertyValues());
}

std::vector<std::shared_ptr<Quest>> QuestWeaver::GetQuestsWithState(QuestState state) const {
    return quests->GetQuestsWithState(state);
}

void QuestWeaver::Tick(float delta) {
    for (const auto &quest : quests->GetQuests()) {
        const QuestTickResult &change = quest->Tick(delta);
        world->Execute(change.GetWorldChanges());
        quests->Execute(change.GetQuestChange());
    }
}

void QuestWeaver::RegisterWorldListener(std::shared_ptr<WorldListener> listener) {
    world->AddListener(listener);
}

std::vector<std::shared_ptr<Quest>> QuestWeaver::GetAllQuests() const {
    return quests->GetQuests();
}

shared_ptr<Quest> QuestWeaver::GetQuest(ID questId) const {
    return quests->GetQuest(questId);
}

std::shared_ptr<Quest> QuestWeaver::ChangeQuestState(QuestModelAction questAction) {
    return quests->Execute(questAction);
}
