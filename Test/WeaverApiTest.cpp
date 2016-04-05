//
// Created by michael on 12.10.15.
//

#include "catch.hpp"
#include <Core/WeaverUtils.h>
#include <QuestWeaver.h>
#include "Mock/TestHelper.h"

using namespace weave;
using namespace std;

TEST_CASE("Weaver API errors", "[weaver]") {
    WeaverConfig config = TestHelper::CreateDebugConfig();

    SECTION("No world model") {
        config.worldModel.reset(nullptr);
        REQUIRE_THROWS_AS(QuestWeaver weaver(config), ContractFailedException);
    }

    SECTION("Unknown serialization type") {
        QuestWeaver weaver(config);
        stringstream ss;
        StreamType type = (StreamType) 100;
        REQUIRE_THROWS_AS(weaver.Serialize(ss, type), ContractFailedException);
    }

    SECTION("Unknown deserialization type") {
        stringstream ss2;
        QuestWeaver weaver(config);
        weaver.Serialize(ss2, StreamType::BINARY);
        string serialized = ss2.str();

        stringstream ss3;
        ss3 << serialized;
        ss3.flush();
        StreamType type = (StreamType) 100;
        REQUIRE_THROWS_AS(QuestWeaver::Deserialize(ss3, type), ContractFailedException);
    }
}

TEST_CASE("Weaver Config", "[weaver]") {
    WeaverConfig configRs = TestHelper::CreateDebugConfig();
    configRs.seed = 0;
    QuestWeaver weaver1(configRs);
    auto quest1 = weaver1.CreateNewQuests()[0];

    WeaverConfig configSeed = TestHelper::CreateDebugConfig();
    configSeed.randomStream = nullptr;

    SECTION("Same seed") {
        configSeed.seed = 42;
        QuestWeaver weaver2(configSeed);
        auto quest2 = weaver2.CreateNewQuests()[0];

        REQUIRE(quest1->GetTitle() == quest2->GetTitle());
        REQUIRE(quest1->GetDescription() == quest2->GetDescription());
    }

    SECTION("Different seed") {
        configSeed.seed = 1337;
        QuestWeaver weaver2(configSeed);
        auto quest2 = weaver2.CreateNewQuests()[0];

        REQUIRE_FALSE(quest1->GetTitle() == quest2->GetTitle());
        REQUIRE_FALSE(quest1->GetDescription() == quest2->GetDescription());
    }
}

TEST_CASE("Weaver Quests", "[weaver]") {
    WeaverConfig config = TestHelper::CreateDebugConfig();
    QuestWeaver weaver(config);

    SECTION("Empty weaver test") {
        REQUIRE(weaver.GetAllQuests().size() == 0);
    }

    auto quest = weaver.CreateNewQuests()[0];
    REQUIRE(quest.get() != nullptr);
    REQUIRE(quest->GetId() != 0);

    SECTION("Created new quests") {
        weaver.CreateNewQuests();
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
        weaver.GetQuest(quest->GetId());
        weaver.ChangeQuestState(failAction);
        weaver.Tick(1);
        REQUIRE(weaver.GetAllQuests().size() == 1);
        REQUIRE(weaver.GetQuestsWithState(QuestState::Inactive).size() == 0);
        REQUIRE(weaver.GetQuestsWithState(QuestState::Failed).size() == 1);
    }
}
