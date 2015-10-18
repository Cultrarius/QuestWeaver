//
// Created by michael on 12.10.15.
//

#include <fstream>
#include "../../QuestWeaver.h"
#include "../catch.hpp"
#include "../../Template/Space/SpaceQuestTemplateFactory.h"
#include "../../World/Space/SpaceWorldModel.h"

using namespace weave;
using namespace std;

TEST_CASE("Serialize Quests", "[serialize]") {
    int testSize = 100;
    shared_ptr<RandomStream> rs = make_shared<RandomStream>(42);
    SpaceQuestTemplateFactory factory(rs);

    SECTION("Serialize and deserialize JSON in-memory") {
        WorldModel *worldModel = new SpaceWorldModel(rs);
        for (int i = 0; i < testSize; i++) {
            rs->Seed(i);
            for (string templateKey : factory.GetTemplateKeys()) {
                auto tp = factory.CreateTemplate(templateKey);
                vector<QuestPropertyValue> questValues;
                for (auto property : tp->GetProperties()) {
                    if (!property.IsMandatory()) {
                        continue;
                    }
                    auto candidates = tp->GetPropertyCandidates(property, *worldModel);
                    for (auto candidate : candidates) {
                        questValues.push_back(QuestPropertyValue(property, candidate.GetEntity()));
                    }
                }

                INFO("Template Key: " + templateKey + ", Seed: " + to_string(i));
                auto originalQuest = tp->ToQuest(questValues);

                stringstream ss;
                {
                    cereal::JSONOutputArchive outputArchive(ss);
                    outputArchive(originalQuest);
                }
                string serialized = ss.str();
                REQUIRE(!serialized.empty());

                shared_ptr<Quest> deserializedQuest;
                {
                    cereal::JSONInputArchive inputArchive(ss);
                    inputArchive(deserializedQuest);
                }

                REQUIRE(deserializedQuest.get() != nullptr);
                REQUIRE(deserializedQuest->GetId() == originalQuest->GetId());
                REQUIRE(deserializedQuest->GetDescription() == originalQuest->GetDescription());
                REQUIRE(deserializedQuest->GetState() == originalQuest->GetState());
                REQUIRE(deserializedQuest->GetTitle() == originalQuest->GetTitle());
            }
        }
    }

    SECTION("Serialize and deserialize binary file") {
        WorldModel *worldModel = new SpaceWorldModel(rs);
        const char *fileName = "testSerialization.bin";
        for (int i = 0; i < testSize; i++) {
            rs->Seed(i);
            for (string templateKey : factory.GetTemplateKeys()) {
                auto tp = factory.CreateTemplate(templateKey);
                vector<QuestPropertyValue> questValues;
                for (auto property : tp->GetProperties()) {
                    if (!property.IsMandatory()) {
                        continue;
                    }
                    auto candidates = tp->GetPropertyCandidates(property, *worldModel);
                    for (auto candidate : candidates) {
                        questValues.push_back(QuestPropertyValue(property, candidate.GetEntity()));
                    }
                }

                INFO("Template Key: " + templateKey + ", Seed: " + to_string(i));
                auto originalQuest = tp->ToQuest(questValues);

                {
                    ofstream os(fileName);
                    cereal::PortableBinaryOutputArchive outputArchive(os);
                    outputArchive(originalQuest);
                }

                shared_ptr<Quest> deserializedQuest;
                {
                    ifstream is(fileName);
                    cereal::PortableBinaryInputArchive inputArchive(is);
                    inputArchive(deserializedQuest);
                }

                REQUIRE(deserializedQuest.get() != nullptr);
                REQUIRE(deserializedQuest->GetId() == originalQuest->GetId());
                REQUIRE(deserializedQuest->GetDescription() == originalQuest->GetDescription());
                REQUIRE(deserializedQuest->GetState() == originalQuest->GetState());
                REQUIRE(deserializedQuest->GetTitle() == originalQuest->GetTitle());
            }
        }
        remove(fileName);
    }

    SECTION("Serialize and deserialize quest model") {
        QuestModel questModel;
        WorldModel *worldModel = new SpaceWorldModel(rs);

        for (int i = 0; i < testSize; i++) {
            rs->Seed(i);
            for (string templateKey : factory.GetTemplateKeys()) {
                auto tp = factory.CreateTemplate(templateKey);
                vector<QuestPropertyValue> questValues;
                for (auto property : tp->GetProperties()) {
                    if (!property.IsMandatory()) {
                        continue;
                    }
                    auto candidates = tp->GetPropertyCandidates(property, *worldModel);
                    for (auto candidate : candidates) {
                        questValues.push_back(QuestPropertyValue(property, candidate.GetEntity()));
                    }
                }

                INFO("Template Key: " + templateKey + ", Seed: " + to_string(i));
                try {
                    auto quest = tp->ToQuest(questValues);
                    questModel.RegisterQuest(*quest);
                } catch (ContractFailedException ex) {
                    FAIL(ex.what());
                }
            }
        }

        stringstream ss;
        {
            cereal::JSONOutputArchive outputArchive(ss);
            outputArchive(questModel);
        }
        string serialized = ss.str();
        REQUIRE(!serialized.empty());

        QuestModel deserializedModel;
        {
            cereal::JSONInputArchive inputArchive(ss);
            inputArchive(deserializedModel);
        }

        REQUIRE(deserializedModel.GetQuests().size() == questModel.GetQuests().size());

        for (int i = 0; i < testSize; i++) {
            REQUIRE(deserializedModel.GetQuests()[i]->GetId() == questModel.GetQuests()[i]->GetId());
        }
    }
}
