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
    engine.reset(new WeaverEngine());
    quests.reset(new QuestModel());
    templates.reset(new TemplateEngine());
    world.reset(new SpaceWorldModel(randomStream));

    shared_ptr<TemplateFactory> spaceFactory = make_shared<SpaceQuestTemplateFactory>(randomStream);
    templates->RegisterTemplateFactory(spaceFactory);
}

shared_ptr<Quest> QuestWeaver::CreateNewQuest() {
    auto questTemplate = templates->GetTemplateForNewQuest(randomStream);
    std::vector<WorldModelAction> modelActions;
    vector<QuestPropertyValue> questPropertyValues = engine->fillTemplate(questTemplate, *quests, *world, randomStream,
                                                                          &modelActions);
    shared_ptr<Quest> newQuest = questTemplate->ToQuest(questPropertyValues);
    // TODO create quest-variants and choose the best one?

    updateWorld(modelActions, QuestModelAction(QuestActionType::REGISTER, newQuest, questPropertyValues));
    return newQuest;
}

void QuestWeaver::updateWorld(const vector<WorldModelAction> &modelActions,
                              const QuestModelAction &questModelAction) {
    world->Execute(modelActions);
    quests->Execute(questModelAction);
}

std::vector<std::shared_ptr<Quest>> QuestWeaver::GetQuestsWithState(QuestState state) const {
    return quests->GetQuestsWithState(state);
}

void QuestWeaver::Tick(float delta) {
    std::vector<WorldModelAction> actions;
    for (const auto &quest : quests->GetQuests()) {
        for (const auto &change : quest->Tick(delta)) {
            actions.push_back(move(change));
        }
    }

    if (actions.size() > 0) {
        world->Execute(actions);
    }
}
