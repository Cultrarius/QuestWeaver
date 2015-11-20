//
// Created by michael on 12.10.15.
//

#include <string>
#include "catch.hpp"
#include "../Core/WeaverUtils.h"
#include "../QuestModel/QuestModel.h"
#include "Mock/TestQuest.h"

using namespace weave;
using namespace std;

TEST_CASE("Quest Model", "[model]") {
    QuestModel model;

    SECTION("Empty model test") {
        REQUIRE(model.GetQuests().size() == 0);
    }

    vector<QuestPropertyValue> properties;
    TestQuest newQuest("TestTitle", "Blabla");

    SECTION("Register quest") {
        REQUIRE(newQuest.GetId() == 0);
        REQUIRE(newQuest.GetState() == QuestState::Proposed);
        REQUIRE(newQuest.GetTitle() == "TestTitle");
        REQUIRE(newQuest.GetDescription() == "Blabla");
        auto quest = model.RegisterQuest(newQuest, properties);
        REQUIRE(quest->GetId() != 0);
        REQUIRE(quest->GetState() == QuestState::Inactive);
        REQUIRE(quest->GetTitle() == "TestTitle");
        REQUIRE(quest->GetDescription() == "Blabla");
    }

    auto quest = model.RegisterQuest(newQuest, properties);
    REQUIRE(quest->GetId() != 0);

    SECTION("Model with single inactive quest") {
        REQUIRE(model.GetQuests().size() == 1);
        REQUIRE(model.GetQuestsWithState(QuestState::Inactive).size() == 1);
    }
}
