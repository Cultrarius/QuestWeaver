//
// Created by michael on 12.10.15.
//

#include <string>
#include "catch.hpp"
#include "../Core/WeaverUtils.h"
#include "../QuestModel/QuestModel.h"
#include "Mock/TestQuest.h"
#include "Mock/TestEntity.h"
#include "../QuestWeaver.h"

using namespace weave;
using namespace std;

TEST_CASE("Weaver Quests", "[weaver]") {
    QuestWeaver weaver(2);

    SECTION("Empty weaver test") {
        REQUIRE(weaver.GetAllQuests().size() == 0);
    }

    auto quest = weaver.CreateNewQuest();
    REQUIRE(quest.get() != nullptr);
    REQUIRE(quest->GetId() != 0);

    SECTION("Created new quests") {
        weaver.CreateNewQuest();
        REQUIRE(weaver.GetAllQuests().size() == 2);
        REQUIRE(weaver.GetQuestsWithState(QuestState::Inactive).size() == 2);
    }

    SECTION("Simple Tick without changes") {
        weaver.Tick(1);
        REQUIRE(weaver.GetAllQuests().size() == 1);
        REQUIRE(weaver.GetQuestsWithState(QuestState::Inactive).size() == 1);
    }

    QuestModelAction activateAction(QuestActionType::ACTIVATE, quest->GetId());
    QuestModelAction failAction(QuestActionType::FAIL, quest->GetId());

    SECTION("Tick activate quest") {
        weaver.ChangeQuestState(activateAction);
        weaver.Tick(1);
        REQUIRE(weaver.GetAllQuests().size() == 1);
        REQUIRE(weaver.GetQuestsWithState(QuestState::Inactive).size() == 0);
        REQUIRE(weaver.GetQuestsWithState(QuestState::Active).size() == 1);
    }

    SECTION("Ignored Tick fail quest") {
        weaver.ChangeQuestState(failAction);
        weaver.Tick(1);
        REQUIRE(weaver.GetAllQuests().size() == 1);
        REQUIRE(weaver.GetQuestsWithState(QuestState::Inactive).size() == 1);
        REQUIRE(weaver.GetQuestsWithState(QuestState::Failed).size() == 0);
    }

    SECTION("Tick fail quest") {
        weaver.ChangeQuestState(activateAction);
        weaver.Tick(1);
        quest = weaver.GetQuest(quest->GetId());
        weaver.ChangeQuestState(failAction);
        weaver.Tick(1);
        REQUIRE(weaver.GetAllQuests().size() == 1);
        REQUIRE(weaver.GetQuestsWithState(QuestState::Inactive).size() == 0);
        REQUIRE(weaver.GetQuestsWithState(QuestState::Failed).size() == 1);
    }
}
