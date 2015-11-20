//
// Created by michael on 12.10.15.
//

#include <string>
#include "catch.hpp"
#include "../Core/WeaverUtils.h"
#include "../QuestModel/QuestModel.h"
#include "Mock/TestQuest.h"
#include "Mock/TestEntity.h"

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

        SECTION("activate quest") {
            REQUIRE(!model.FailQuest(quest->GetId()));
            REQUIRE(model.GetQuestsWithState(QuestState::Inactive).size() == 1);
            REQUIRE(model.GetQuestsWithState(QuestState::Failed).size() == 0);

            REQUIRE(!model.SucceedQuest(quest->GetId()));
            REQUIRE(model.GetQuestsWithState(QuestState::Inactive).size() == 1);
            REQUIRE(model.GetQuestsWithState(QuestState::Success).size() == 0);

            REQUIRE(model.ActivateQuest(quest->GetId()));
            REQUIRE(model.GetQuestsWithState(QuestState::Inactive).size() == 0);
            REQUIRE(model.GetQuestsWithState(QuestState::Active).size() == 1);
        }

        SECTION("Fail quest") {
            REQUIRE(model.ActivateQuest(quest->GetId()));
            REQUIRE(model.FailQuest(quest->GetId()));
            REQUIRE(model.GetQuestsWithState(QuestState::Inactive).size() == 0);
            REQUIRE(model.GetQuestsWithState(QuestState::Active).size() == 0);
            REQUIRE(model.GetQuestsWithState(QuestState::Failed).size() == 1);
        }

        SECTION("Success quest") {
            REQUIRE(model.ActivateQuest(quest->GetId()));
            REQUIRE(model.SucceedQuest(quest->GetId()));
            REQUIRE(model.GetQuestsWithState(QuestState::Inactive).size() == 0);
            REQUIRE(model.GetQuestsWithState(QuestState::Active).size() == 0);
            REQUIRE(model.GetQuestsWithState(QuestState::Success).size() == 1);
        }
    }

    TestQuest newQuest2("TestTitle2", "Blabla2");
    TemplateQuestProperty templateValue(true, "testProperty");
    shared_ptr<WorldEntity> entity = make_shared<TestEntity>();
    QuestPropertyValue value(templateValue, entity);
    properties.push_back(value);
    auto quest2 = model.RegisterQuest(newQuest2, properties);
    REQUIRE(quest2->GetId() != 0);

    SECTION("Get quest entities") {
        REQUIRE(model.GetQuestEntities(quest->GetId()).size() == 0);
        REQUIRE(model.GetQuestEntities(quest2->GetId()).size() == 1);
        REQUIRE(*(model.GetQuestEntities(quest2->GetId()).begin()) == entity);
    }
}
