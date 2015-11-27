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
    shared_ptr<Quest> newQuest = make_shared<TestQuest>("TestTitle", "Blabla");

    SECTION("Keep unknown quest") {
        auto keepAction = QuestModelAction(QuestActionType::KEEP, 123);
        REQUIRE_THROWS_AS(model.Execute(keepAction), ContractFailedException);
    }

    SECTION("Register quest") {
        REQUIRE(newQuest->GetId() == 0);
        REQUIRE(newQuest->GetState() == QuestState::Proposed);
        REQUIRE(newQuest->GetTitle() == "TestTitle");
        REQUIRE(newQuest->GetDescription() == "Blabla");
        auto quest = model.RegisterNew(newQuest, properties);
        REQUIRE(quest->GetId() != 0);
        REQUIRE(quest->GetState() == QuestState::Inactive);
        REQUIRE(quest->GetTitle() == "TestTitle");
        REQUIRE(quest->GetDescription() == "Blabla");
    }

    auto quest = model.RegisterNew(newQuest, properties);
    REQUIRE(quest->GetId() != 0);

    SECTION("Register quest twice") {
        REQUIRE_THROWS_AS(model.RegisterNew(quest, properties), ContractFailedException);
    }

    SECTION("Keep known quest") {
        auto keepAction = QuestModelAction(QuestActionType::KEEP, quest->GetId());
        REQUIRE(model.Execute(keepAction) == quest);
    }

    SECTION("Model with single inactive quest") {
        REQUIRE(model.GetQuests().size() == 1);
        REQUIRE(model.GetQuestsWithState(QuestState::Inactive).size() == 1);

        QuestModelAction activateAction(QuestActionType::ACTIVATE, quest->GetId());
        QuestModelAction failAction(QuestActionType::FAIL, quest->GetId());
        QuestModelAction succeedAction(QuestActionType::SUCCEED, quest->GetId());

        SECTION("activate quest") {
            REQUIRE(!model.Execute(failAction));
            REQUIRE(model.GetQuestsWithState(QuestState::Inactive).size() == 1);
            REQUIRE(model.GetQuestsWithState(QuestState::Failed).size() == 0);

            REQUIRE(!model.Execute(succeedAction));
            REQUIRE(model.GetQuestsWithState(QuestState::Inactive).size() == 1);
            REQUIRE(model.GetQuestsWithState(QuestState::Success).size() == 0);

            REQUIRE(model.Execute(activateAction));
            REQUIRE(model.GetQuestsWithState(QuestState::Inactive).size() == 0);
            REQUIRE(model.GetQuestsWithState(QuestState::Active).size() == 1);
        }

        SECTION("Fail quest") {
            REQUIRE(model.Execute(activateAction));
            REQUIRE(model.Execute(failAction));
            REQUIRE(model.GetQuestsWithState(QuestState::Inactive).size() == 0);
            REQUIRE(model.GetQuestsWithState(QuestState::Active).size() == 0);
            REQUIRE(model.GetQuestsWithState(QuestState::Failed).size() == 1);
        }

        SECTION("Success quest") {
            REQUIRE(model.Execute(activateAction));
            REQUIRE(model.Execute(succeedAction));
            REQUIRE(model.GetQuestsWithState(QuestState::Inactive).size() == 0);
            REQUIRE(model.GetQuestsWithState(QuestState::Active).size() == 0);
            REQUIRE(model.GetQuestsWithState(QuestState::Success).size() == 1);
        }
    }

    shared_ptr<Quest> newQuest2 = make_shared<TestQuest>("TestTitle2", "Blabla2");
    TemplateQuestProperty templateValue(true, "testProperty");
    shared_ptr<WorldEntity> entity = make_shared<TestEntity>();
    QuestPropertyValue value(templateValue, entity);
    properties.push_back(value);
    auto quest2 = model.RegisterNew(newQuest2, properties);
    REQUIRE(quest2->GetId() != 0);

    SECTION("Get quest entities") {
        REQUIRE(model.GetQuestEntities(quest->GetId()).size() == 0);
        REQUIRE(model.GetQuestEntities(quest2->GetId()).size() == 1);
        REQUIRE(*(model.GetQuestEntities(quest2->GetId()).begin()) == entity);
    }
}
