//
// Created by michael on 12.10.15.
//

#include <string>
#include <memory>
#include <Template/TemplateEngine.h>
#include <Core/WeaverUtils.h>
#include <Template/Space/SpaceQuestTemplateFactory.h>
#include <World/Space/SpaceWorldModel.h>
#include "../catch.hpp"

using namespace weave;
using namespace std;

TEST_CASE("Format check", "[template]") {
    shared_ptr<RandomStream> rs = make_shared<RandomStream>(42);
    TemplateEngine textEngine(rs, Directories(), FormatterType::TEXT);
    TemplateEngine htmlEngine(rs, Directories(), FormatterType::HTML);
    REQUIRE(textEngine.GetFormat() == FormatterType::TEXT);
    REQUIRE(htmlEngine.GetFormat() == FormatterType::HTML);
}

TEST_CASE("Template factory", "[template]") {
    shared_ptr<RandomStream> rs = make_shared<RandomStream>(42);
    TemplateEngine engine(rs, Directories(), FormatterType::TEXT);
    REQUIRE_THROWS_AS(engine.GetTemplateForNewQuest(), ContractFailedException);

    shared_ptr<SpaceQuestTemplateFactory> factory = make_shared<SpaceQuestTemplateFactory>();
    engine.RegisterTemplateFactory(factory);
    REQUIRE(factory->GetTemplateKeys().size() >= 1);

    SECTION("Checking the keys") {
        vector<string> keys = {"ExploreRegionQuest"};
        for (string key : keys) {
            bool hasKey = false;
            for (string templateKey : factory->GetTemplateKeys()) {
                if (key == templateKey) {
                    hasKey = true;
                    break;
                }
            }
            REQUIRE(hasKey);
        }
    }

    SECTION("Retrieving template for every key") {
        for (string templateKey : factory->GetTemplateKeys()) {
            auto temp = factory->CreateTemplate(templateKey);
            REQUIRE(temp.get() != nullptr);
        }
    }
}

TEST_CASE("Templates", "[template]") {
    int testSize = 100;
    shared_ptr<RandomStream> rs = make_shared<RandomStream>(42);
    TemplateEngine engine(rs, Directories(), FormatterType::TEXT);
    shared_ptr<SpaceQuestTemplateFactory> factory = make_shared<SpaceQuestTemplateFactory>();
    engine.RegisterTemplateFactory(factory);

    SECTION("Checking properties size") {
        for (int i = 0; i < testSize; i++) {
            rs->Seed(i);
            for (string templateKey : factory->GetTemplateKeys()) {
                auto tp = factory->CreateTemplate(templateKey);
                INFO("Template Key: " + templateKey + ", Seed: " + to_string(i));
                REQUIRE(tp->GetProperties().size() > 0);
            }
        }
    }

    SECTION("Checking mandatory property exists") {
        for (int i = 0; i < testSize; i++) {
            rs->Seed(i);
            for (string templateKey : factory->GetTemplateKeys()) {
                auto tp = factory->CreateTemplate(templateKey);
                bool hasMandatoryProperty = false;
                for (auto property : tp->GetProperties()) {
                    if (property.IsMandatory()) {
                        hasMandatoryProperty = true;
                        break;
                    }
                }
                INFO("Template Key: " + templateKey + ", Seed: " + to_string(i));
                REQUIRE(hasMandatoryProperty);
            }
        }
    }

    SECTION("Checking property names") {
        for (int i = 0; i < testSize; i++) {
            rs->Seed(i);
            for (string templateKey : factory->GetTemplateKeys()) {
                auto tp = factory->CreateTemplate(templateKey);
                for (auto property : tp->GetProperties()) {
                    INFO("Template Key: " + templateKey + ", Property: " + property.GetName() + ", Seed: " +
                         to_string(i));
                    REQUIRE(!property.GetName().empty());
                }
            }
        }
    }

    SECTION("Checking property candidates") {
        WorldModel *worldModel = new SpaceWorldModel(rs);
        for (int i = 0; i < testSize; i++) {
            rs->Seed(i);
            for (string templateKey : factory->GetTemplateKeys()) {
                auto tp = factory->CreateTemplate(templateKey);
                for (auto property : tp->GetProperties()) {
                    auto candidates = tp->GetPropertyCandidates(property, *worldModel);
                    INFO("Template Key: " + templateKey + ", Property: " + property.GetName() + ", Seed: " +
                         to_string(i));
                    REQUIRE(candidates.size() > 0);
                }
            }
        }
    }

    SECTION("Checking model actions on empty model ") {
        WorldModel *worldModel = new SpaceWorldModel(rs);
        for (int i = 0; i < testSize; i++) {
            rs->Seed(i);
            for (string templateKey : factory->GetTemplateKeys()) {
                auto tp = factory->CreateTemplate(templateKey);
                for (auto property : tp->GetProperties()) {
                    auto candidates = tp->GetPropertyCandidates(property, *worldModel);
                    for (auto candidate : candidates) {
                        INFO("Template Key: " + templateKey + ", Property: " + property.GetName() + ", Seed: " +
                             to_string(i));
                        REQUIRE(candidate.GetActionType() == WorldActionType::CREATE);
                        REQUIRE(candidate.GetEntity().get() != nullptr);
                    }
                }
            }
        }
    }

    SECTION("Checking quest creation all candidates") {
        WorldModel *worldModel = new SpaceWorldModel(rs);
        for (int i = 0; i < testSize; i++) {
            rs->Seed(i);
            for (string templateKey : factory->GetTemplateKeys()) {
                auto tp = factory->CreateTemplate(templateKey);
                vector<QuestPropertyValue> questValues;
                for (auto property : tp->GetProperties()) {
                    auto candidates = tp->GetPropertyCandidates(property, *worldModel);
                    for (auto candidate : candidates) {
                        questValues.push_back(QuestPropertyValue(property, candidate.GetEntity()));
                    }
                    worldModel->Execute(candidates);
                }

                INFO("Template Key: " + templateKey + ", Seed: " + to_string(i));
                try {
                    auto quest = tp->ToQuest(questValues);
                    REQUIRE(!quest->GetDescription().empty());
                    REQUIRE(quest->GetState() == QuestState::Proposed);
                    REQUIRE(!quest->GetTitle().empty());
                } catch (ContractFailedException ex) {
                    FAIL(ex.what());
                }
            }
        }
    }

    SECTION("Checking quest creation only mandatory candidates") {
        WorldModel *worldModel = new SpaceWorldModel(rs);
        for (int i = 0; i < testSize; i++) {
            rs->Seed(i);
            for (string templateKey : factory->GetTemplateKeys()) {
                auto tp = factory->CreateTemplate(templateKey);
                vector<QuestPropertyValue> questValues;
                for (auto property : tp->GetProperties()) {
                    if (!property.IsMandatory()) {
                        continue;
                    }
                    auto candidates = tp->GetPropertyCandidates(property, *worldModel);
                    for (auto candidate : candidates) {
                        questValues.push_back(QuestPropertyValue(property, candidate.GetEntity()));
                    }
                    worldModel->Execute(candidates);
                }

                INFO("Template Key: " + templateKey + ", Seed: " + to_string(i));
                try {
                    auto quest = tp->ToQuest(questValues);
                    REQUIRE(!quest->GetDescription().empty());
                    REQUIRE(quest->GetState() == QuestState::Proposed);
                    REQUIRE(!quest->GetTitle().empty());
                } catch (ContractFailedException ex) {
                    FAIL(ex.what());
                }
            }
        }
    }
}

TEST_CASE("Directory Change", "[template]") {
    shared_ptr<RandomStream> rs = make_shared<RandomStream>(42);
    TemplateEngine engine(rs, Directories(), FormatterType::TEXT);
    shared_ptr<SpaceQuestTemplateFactory> factory = make_shared<SpaceQuestTemplateFactory>();

    SECTION("Unknown dir pre reg") {
        Directories unknown;
        unknown.templateDirectory = "Test/Unknown/";
        unknown.modDirectory = "../Test/Unknown/";
        engine.ChangeDirectories(unknown);
        engine.RegisterTemplateFactory(factory);
        REQUIRE_THROWS_AS(engine.GetTemplateForNewQuest(), ContractFailedException);
    }

    SECTION("Unknown dir post reg") {
        engine.RegisterTemplateFactory(factory);
        Directories unknown;
        unknown.templateDirectory = "Test/Unknown/";
        unknown.modDirectory = "../Test/Unknown/";
        engine.ChangeDirectories(unknown);
        REQUIRE_THROWS_AS(engine.GetTemplateForNewQuest(), ContractFailedException);
    }

    SECTION("Pre register") {
        Directories unknown;
        unknown.templateDirectory = "Test/Resources/";
        unknown.modDirectory = "../Test/Resources/";
        engine.ChangeDirectories(unknown);
        engine.RegisterTemplateFactory(factory);
        REQUIRE(engine.GetTemplateForNewQuest() != nullptr);
    }
}
