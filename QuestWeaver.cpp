//
// Created by michael on 10.08.15.
//

#include "QuestWeaver.h"

using namespace std;

QuestWeaver::QuestWeaver() {
    engine = unique_ptr<WeaverEngine>(new WeaverEngine());
    quests = unique_ptr<QuestModel>(new QuestModel());
    templates = unique_ptr<TemplateEngine>(new TemplateEngine());
    world = unique_ptr<WorldModel>(new WorldModel());
}

std::list<Quest> QuestWeaver::GetActiveQuests() const {
    return quests->getActiveQuests();
}

Quest QuestWeaver::CreateNewQuest() {
    Template questTemplate = templates->GetTemplateForNewQuest();
    fillTemplate(questTemplate);
    Quest newQuest = questTemplate.ToQuest();
    //TODO create quest-variants and choose the best one
    updateWorld(newQuest);
    return newQuest;
}

void QuestWeaver::fillTemplate(Template &questTemplate) {

}

void QuestWeaver::updateWorld(Quest &quest) {

}
