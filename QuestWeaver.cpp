//
// Created by michael on 10.08.15.
//

#include "QuestWeaver.h"
#include "Template/Space/SpaceQuestTemplateFactory.h"
#include <algorithm>
#include <iostream>

using namespace std;

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
    vector<QuestPropertyValue> questPropertyValues = fillTemplate(questTemplate);
    Quest newQuest = questTemplate->ToQuest(questPropertyValues);
    //TODO create quest-variants and choose the best one
    updateWorld(newQuest);
    return newQuest;
}

vector<QuestPropertyValue> QuestWeaver::fillTemplate(shared_ptr<Template> &questTemplate) {
    //TODO insert fancy algorithm
    vector<QuestPropertyValue> returnValues;
    vector<TemplateQuestProperty> propertiesToCreate;
    for (auto property : questTemplate->GetProperties()) {
        if (property.IsMandatory() || rand() % 100 < 50) {
            propertiesToCreate.push_back(property);
        } else {
            cout << "Omitting property " << property.GetName() << endl;
        }
    }

    for (auto property : propertiesToCreate) {
        const vector<ModelAction> candidates = questTemplate->GetPropertyCandidates(property, *world);

        shared_ptr<WorldEntity> entity = candidates[0].GetEntity();
        QuestPropertyValue questValue(property, entity);
    }

    return returnValues;
}

void QuestWeaver::updateWorld(Quest &quest) {

}
