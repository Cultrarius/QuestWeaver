//
// Created by michael on 12.10.15.
//

#include "catch.hpp"
#include "Mock/TestStoryTemplateFactory.h"
#include "Mock/TestEntity.h"
#include "Mock/TestHelper.h"
#include <Story/StoryWriter.h>
#include <Story/Space/CommonSpaceStoryFactory.h>
#include <World/Space/SpaceWorldModel.h>
#include <WeaverConfig.h>

using namespace weave;
using namespace std;

TEST_CASE("Nugget Randomizer", "[story]") {
    string key = "testNugget";
    vector<string> requiredTypes;
    vector<string> texts = {"Lorem ipsum"};
    unordered_map<string, int> minValues;
    unordered_map<string, int> maxValues;
    unordered_map<string, NameType> randomNames;
    shared_ptr<RandomStream> rs = make_shared<RandomStream>(38);
    SpaceWorldModel worldModel(rs);

    SECTION("Empty nugget test") {
        auto n = new Nugget(key, 1, requiredTypes, texts, minValues, maxValues, randomNames);
        REQUIRE(key == n->GetKey());
        REQUIRE(0 == n->GetRandomizationKeys().size());
        REQUIRE(0 == n->GetRequiredTypes().size());
        REQUIRE(1 == n->GetTexts().size());
        REQUIRE("" == n->GetRandomValue("asd", rs, worldModel.GetNameGenerator()));
    }

    SECTION("Random name test") {
        randomNames["123test"] = NameType::FUNNY;
        auto n = new Nugget(key, 1, requiredTypes, texts, minValues, maxValues, randomNames);
        REQUIRE(key == n->GetKey());
        REQUIRE(1 == n->GetRandomizationKeys().size());
        REQUIRE("123test" == *n->GetRandomizationKeys().begin());
        REQUIRE("" != n->GetRandomValue("123test", rs, worldModel.GetNameGenerator()));
    }

    SECTION("min value") {
        minValues["123test"] = 50;
        auto n = new Nugget(key, 1, requiredTypes, texts, minValues, maxValues, randomNames);
        REQUIRE(key == n->GetKey());
        REQUIRE(1 == n->GetRandomizationKeys().size());
        REQUIRE("123test" == *n->GetRandomizationKeys().begin());
        for (int i = 0; i < 1000; i++) {
            string str = n->GetRandomValue("123test", rs, worldModel.GetNameGenerator());
            int val = atoi(str.c_str());
            REQUIRE(val >= 50);
        }
    }

    SECTION("big min value") {
        minValues["123test"] = 5000;
        auto n = new Nugget(key, 1, requiredTypes, texts, minValues, maxValues, randomNames);
        REQUIRE(key == n->GetKey());
        REQUIRE(1 == n->GetRandomizationKeys().size());
        REQUIRE("123test" == *n->GetRandomizationKeys().begin());
        for (int i = 0; i < 1000; i++) {
            string str = n->GetRandomValue("123test", rs, worldModel.GetNameGenerator());
            int val = atoi(str.c_str());
            REQUIRE(val == 5000);
        }
    }

    SECTION("max value") {
        maxValues["123test"] = 50;
        auto n = new Nugget(key, 1, requiredTypes, texts, minValues, maxValues, randomNames);
        REQUIRE(key == n->GetKey());
        REQUIRE(1 == n->GetRandomizationKeys().size());
        REQUIRE("123test" == *n->GetRandomizationKeys().begin());
        for (int i = 0; i < 1000; i++) {
            string str = n->GetRandomValue("123test", rs, worldModel.GetNameGenerator());
            int val = atoi(str.c_str());
            REQUIRE(val <= 50);
        }
    }

    SECTION("small max value") {
        maxValues["123test"] = -10;
        auto n = new Nugget(key, 1, requiredTypes, texts, minValues, maxValues, randomNames);
        REQUIRE(key == n->GetKey());
        REQUIRE(1 == n->GetRandomizationKeys().size());
        REQUIRE("123test" == *n->GetRandomizationKeys().begin());
        for (int i = 0; i < 1000; i++) {
            string str = n->GetRandomValue("123test", rs, worldModel.GetNameGenerator());
            int val = atoi(str.c_str());
            REQUIRE(val == -10);
        }
    }

    SECTION("Same min/max value") {
        minValues["123test"] = 50;
        maxValues["123test"] = 50;
        auto n = new Nugget(key, 1, requiredTypes, texts, minValues, maxValues, randomNames);
        REQUIRE(key == n->GetKey());
        REQUIRE(1 == n->GetRandomizationKeys().size());
        REQUIRE("123test" == *n->GetRandomizationKeys().begin());
        for (int i = 0; i < 1000; i++) {
            string str = n->GetRandomValue("123test", rs, worldModel.GetNameGenerator());
            int val = atoi(str.c_str());
            REQUIRE(val == 50);
        }
    }

    SECTION("min/max value") {
        minValues["123test"] = 1000;
        maxValues["123test"] = 50000;
        auto n = new Nugget(key, 1, requiredTypes, texts, minValues, maxValues, randomNames);
        REQUIRE(key == n->GetKey());
        REQUIRE(1 == n->GetRandomizationKeys().size());
        REQUIRE("123test" == *n->GetRandomizationKeys().begin());
        for (int i = 0; i < 1000; i++) {
            string str = n->GetRandomValue("123test", rs, worldModel.GetNameGenerator());
            int val = atoi(str.c_str());
            REQUIRE(val >= 1000);
            REQUIRE(val <= 50000);
        }
    }
}

TEST_CASE("Nuggets", "[story]") {
    Directories dirs;
    dirs.templateDirectory = "Test/Resources/";
    dirs.modDirectory = "../Test/Resources/";
    shared_ptr<RandomStream> rs = make_shared<RandomStream>(42);
    TemplateEngine engine(rs, dirs, FormatterType::TEXT);
    QuestModel questModel;
    SpaceWorldModel worldModel(rs);
    WeaverGraph graph;
    vector<QuestPropertyValue> values;
    StoryWriter writer(rs, questModel, engine, worldModel, dirs);

    SECTION("Empty story test") {
        auto result = writer.CreateStory(StoryWriterParameters(graph, values));
        REQUIRE("" == result.text);
    }

    SECTION("Valid nuggets") {
        writer.RegisterTemplateFactory(unique_ptr<StoryTemplateFactory>(new TestStoryTemplateFactory("1")));
        auto result = writer.CreateStory(StoryWriterParameters(graph, values));
        REQUIRE("" == result.text);
    }

    SECTION("Invalid nuggets - no key") {
        writer.RegisterTemplateFactory(unique_ptr<StoryTemplateFactory>(new TestStoryTemplateFactory("2")));
        REQUIRE_THROWS_AS(writer.CreateStory(StoryWriterParameters(graph, values)), ContractFailedException);
    }

    SECTION("Invalid nuggets - no required types") {
        writer.RegisterTemplateFactory(unique_ptr<StoryTemplateFactory>(new TestStoryTemplateFactory("3")));
        REQUIRE_THROWS_AS(writer.CreateStory(StoryWriterParameters(graph, values)), ContractFailedException);
    }

    SECTION("Invalid nuggets - no texts") {
        writer.RegisterTemplateFactory(unique_ptr<StoryTemplateFactory>(new TestStoryTemplateFactory("3")));
        REQUIRE_THROWS_AS(writer.CreateStory(StoryWriterParameters(graph, values)), ContractFailedException);
    }

    SECTION("Invalid nuggets - no outer json array") {
        writer.RegisterTemplateFactory(unique_ptr<StoryTemplateFactory>(new TestStoryTemplateFactory("6")));
        REQUIRE_THROWS_AS(writer.CreateStory(StoryWriterParameters(graph, values)), ContractFailedException);
    }

    SECTION("Invalid nuggets - no inner json object") {
        writer.RegisterTemplateFactory(unique_ptr<StoryTemplateFactory>(new TestStoryTemplateFactory("7")));
        REQUIRE_THROWS_AS(writer.CreateStory(StoryWriterParameters(graph, values)), ContractFailedException);
    }

    SECTION("Duplicate key") {
        writer.RegisterTemplateFactory(unique_ptr<StoryTemplateFactory>(new TestStoryTemplateFactory("1")));
        writer.RegisterTemplateFactory(unique_ptr<StoryTemplateFactory>(new TestStoryTemplateFactory("5")));
        REQUIRE_THROWS_AS(writer.CreateStory(StoryWriterParameters(graph, values)), ContractFailedException);
    }

    SECTION("Duplicate key, but same folder") {
        writer.RegisterTemplateFactory(unique_ptr<StoryTemplateFactory>(new TestStoryTemplateFactory("1")));
        writer.RegisterTemplateFactory(unique_ptr<StoryTemplateFactory>(new TestStoryTemplateFactory("1")));
        auto result = writer.CreateStory(StoryWriterParameters(graph, values));
        REQUIRE("" == result.text);
    }

    SECTION("Adding agent entity") {

        // create an agent in the world model
        auto agent = worldModel.CreateAgent().GetEntity();
        TemplateQuestProperty templateProperty(true, "superhero");
        values.push_back(QuestPropertyValue(templateProperty, agent));
        set<string> requiredTypes = {"agent"};

        // create a fitting story template factory
        writer.RegisterTemplateFactory(
                unique_ptr<StoryTemplateFactory>(new TestStoryTemplateFactory("1", requiredTypes)));

        // create a fitting graph node
        graph.CreateNodeGroup("test", true);
        graph.AddNode(Node("test", agent->GetId()));
        graph.Finalize();

        // run it through the story writer
        auto result = writer.CreateStory(StoryWriterParameters(graph, values));
        REQUIRE(result.text == "");
    }
}

TEST_CASE("StoryTemplates", "[story]") {
    shared_ptr<RandomStream> rs = make_shared<RandomStream>(42);
    TemplateEngine engine(rs, Directories(), FormatterType::TEXT);
    QuestModel questModel;
    SpaceWorldModel worldModel(rs);
    WeaverGraph graph;
    vector<QuestPropertyValue> values;
    StoryWriter writer(rs, questModel, engine, worldModel, Directories());

    Directories dirs;
    dirs.templateDirectory = "Test/Resources/";
    dirs.modDirectory = "../Test/Resources/";
    writer.ChangeDirectories(dirs);
    engine.ChangeDirectories(dirs);

    // create a test entity
    auto testEntity = make_shared<TestEntity>();
    TemplateQuestProperty templateProperty(true, "player");
    values.push_back(QuestPropertyValue(templateProperty, testEntity));
    set<string> requiredTypes = {"TestEntityType"};
    WorldModelAction addAction(WorldActionType::CREATE, testEntity);
    worldModel.Execute({addAction});

    // create a fitting graph node
    graph.CreateNodeGroup("player", true);
    graph.AddNode(Node("player", testEntity->GetId()));
    graph.Finalize();

    SECTION("Simple line test") {
        writer.RegisterTemplateFactory(
                unique_ptr<StoryTemplateFactory>(new TestStoryTemplateFactory("8", "storyLines.st")));
        auto result = writer.CreateStory(StoryWriterParameters(graph, values), "simpleLine");
        REQUIRE(result.text == "A. B. C");
    }

    SECTION("Entity line test") {
        writer.RegisterTemplateFactory(
                unique_ptr<StoryTemplateFactory>(new TestStoryTemplateFactory("8", "storyLines.st")));
        auto result = writer.CreateStory(StoryWriterParameters(graph, values), "entityLine");
        REQUIRE(result.text == "I wish me a TestEntity to play with.");
    }

    SECTION("Update deleted entity test") {
        vector<WorldModelAction> questActions;
        questActions.push_back(WorldModelAction(WorldActionType::DELETE, testEntity));
        vector<WorldModelAction> storyActions;
        storyActions.push_back(WorldModelAction(WorldActionType::UPDATE, testEntity));

        writer.RegisterTemplateFactory(
                unique_ptr<StoryTemplateFactory>(new TestStoryTemplateFactory("8", "storyLines.st", storyActions)));

        auto result = writer.CreateStory(StoryWriterParameters(graph, values, questActions), "entityLine");
        REQUIRE(result.text == "");
        REQUIRE(result.worldActions.size() == 0);
    }

    SECTION("String only line test") {
        writer.RegisterTemplateFactory(
                unique_ptr<StoryTemplateFactory>(new TestStoryTemplateFactory("8", "storyLines.st")));
        auto result = writer.CreateStory(StoryWriterParameters(graph, values), "stringOnlyLines");
        REQUIRE(result.text == "This is great! I really love this.");
    }

    SECTION("Unknown nugget") {
        writer.RegisterTemplateFactory(
                unique_ptr<StoryTemplateFactory>(new TestStoryTemplateFactory("8", "storyLines.st")));
        auto result = writer.CreateStory(StoryWriterParameters(graph, values), "unknownNugget");
        REQUIRE(result.text == "");
    }

    SECTION("Nugget IDs mismatch") {
        writer.RegisterTemplateFactory(
                unique_ptr<StoryTemplateFactory>(new TestStoryTemplateFactory("8", "storyLines.st")));
        REQUIRE_THROWS_AS(writer.CreateStory(StoryWriterParameters(graph, values), "wrongNuggetIds"),
                          ContractFailedException);
    }

    SECTION("Nugget content mismatch") {
        writer.RegisterTemplateFactory(
                unique_ptr<StoryTemplateFactory>(new TestStoryTemplateFactory("8", "storyLines.st")));
        REQUIRE_THROWS_AS(writer.CreateStory(StoryWriterParameters(graph, values), "wrongNuggetContent"),
                          ContractFailedException);
    }

    SECTION("Nugget required type mismatch") {
        writer.RegisterTemplateFactory(
                unique_ptr<StoryTemplateFactory>(new TestStoryTemplateFactory("8", "storyLines.st")));
        REQUIRE_THROWS_AS(writer.CreateStory(StoryWriterParameters(graph, values), "wrongType"),
                          ContractFailedException);
    }

    SECTION("Nugget no replacement found") {
        writer.RegisterTemplateFactory(
                unique_ptr<StoryTemplateFactory>(new TestStoryTemplateFactory("8", "storyLines.st")));
        REQUIRE_THROWS_AS(writer.CreateStory(StoryWriterParameters(graph, values), "missingReplacement"),
                          ContractFailedException);
    }

    SECTION("Directory change") {
        writer.RegisterTemplateFactory(
                unique_ptr<StoryTemplateFactory>(new TestStoryTemplateFactory("8", "storyLines.st")));
        auto result = writer.CreateStory(StoryWriterParameters(graph, values), "entityLine");
        REQUIRE(result.text == "I wish me a TestEntity to play with.");
        writer.ChangeDirectories(dirs);
        engine.ChangeDirectories(dirs);
        result = writer.CreateStory(StoryWriterParameters(graph, values), "entityLine");
        REQUIRE(result.text == "I wish me a TestEntity to play with.");

    }

    SECTION("Invalid entity IDs") {
        auto factory = unique_ptr<TestStoryTemplateFactory>(new TestStoryTemplateFactory("8", "storyLines.st"));
        factory->TemplatesReturnInvalidIDs = true;
        writer.RegisterTemplateFactory(move(factory));
        REQUIRE_THROWS_AS(writer.CreateStory(StoryWriterParameters(graph, values), "entityLine"),
                          ContractFailedException);
    }

    SECTION("Broken template") {
        SECTION("No array as outer struct") {
            writer.RegisterTemplateFactory(
                    unique_ptr<StoryTemplateFactory>(new TestStoryTemplateFactory("8", "brokenNoArray.st")));
            REQUIRE_THROWS_AS(writer.CreateStory(StoryWriterParameters(graph, values), "simpleLine"),
                              ContractFailedException);
        }

        SECTION("No Key for story") {
            writer.RegisterTemplateFactory(
                    unique_ptr<StoryTemplateFactory>(new TestStoryTemplateFactory("8", "brokenNoKey.st")));
            REQUIRE_THROWS_AS(writer.CreateStory(StoryWriterParameters(graph, values), "simpleLine"),
                              ContractFailedException);
        }
    }

    SECTION("World Model action result") {
        vector<WorldModelAction> actions;
        actions.push_back(addAction);
        writer.RegisterTemplateFactory(
                unique_ptr<StoryTemplateFactory>(new TestStoryTemplateFactory("8", "storyLines.st", actions)));
        auto result = writer.CreateStory(StoryWriterParameters(graph, values), "entityLine");
        REQUIRE(result.text == "I wish me a TestEntity to play with.");
        REQUIRE(result.worldActions.size() == 1);
        REQUIRE(result.worldActions[0].GetEntity()->GetId() == addAction.GetEntity()->GetId());
    }
}

TEST_CASE("SpaceTemplates", "[story]") {
    WeaverConfig config = TestHelper::CreateDebugConfig();
    shared_ptr<RandomStream> rs = make_shared<RandomStream>(44);
    TemplateEngine engine(rs, config.dirs, FormatterType::TEXT);
    QuestModel questModel;
    SpaceWorldModel worldModel(rs);
    WeaverGraph graph;
    vector<QuestPropertyValue> values;
    StoryWriter writer(rs, questModel, engine, worldModel, config.dirs);

    // create a test entity
    auto newAgentAction = worldModel.CreateAgent();
    auto testAgent = newAgentAction.GetEntity();
    TemplateQuestProperty templateProperty(true, "player");
    values.push_back(QuestPropertyValue(templateProperty, testAgent));
    set<string> requiredTypes = {"agent"};
    worldModel.Execute({newAgentAction});

    // create a fitting graph node
    graph.CreateNodeGroup("player", true);
    graph.AddNode(Node("player", testAgent->GetId()));
    graph.Finalize();

    SECTION("Intro Story") {
        writer.RegisterTemplateFactory(unique_ptr<CommonSpaceStoryFactory>(new CommonSpaceStoryFactory()));
        auto result = writer.CreateStory(StoryWriterParameters(graph, values), "agentIntro");
        INFO(result.text);
        REQUIRE(result.text.length() > 90);
        REQUIRE(result.worldActions[0].GetMetaData().HasValue("introStoryDone"));
    }

    SECTION("No double intro") {
        writer.RegisterTemplateFactory(unique_ptr<CommonSpaceStoryFactory>(new CommonSpaceStoryFactory()));
        auto result = writer.CreateStory(StoryWriterParameters(graph, values), "agentIntro");
        INFO(result.text);
        REQUIRE(result.text.length() > 90);
        worldModel.Execute(result.worldActions);
        result = writer.CreateStory(StoryWriterParameters(graph, values), "agentIntro");
        REQUIRE(result.text == "");
        REQUIRE(result.worldActions.size() == 0);
    }
}

TEST_CASE("Tokenizer", "[story]") {
    vector<RawStoryToken> tokens;

    SECTION("Empty string") {
        tokens = getStoryTokens("");
        REQUIRE(tokens.size() == 0);
    }

    SECTION("Mandatory token") {
        SECTION("Without ID") {
            tokens = getStoryTokens("[sure]");
            REQUIRE(tokens[0].id == "");
            REQUIRE(tokens[0].text == "[sure]");
        }
        SECTION("With ID") {
            tokens = getStoryTokens("[sure:xyz]");
            REQUIRE(tokens[0].id == "xyz");
            REQUIRE(tokens[0].text == "[sure:xyz]");
        }
        SECTION("With text") {
            tokens = getStoryTokens("Before [sure:xyz] and after");
            REQUIRE(tokens[0].id == "xyz");
            REQUIRE(tokens[0].text == "[sure:xyz]");
        }
        SECTION("With line break") {
            tokens = getStoryTokens("Be\tfore\n\r [sure:xyz] and after");
            REQUIRE(tokens[0].id == "xyz");
            REQUIRE(tokens[0].text == "[sure:xyz]");
        }
        REQUIRE(tokens.size() == 1);
        REQUIRE(tokens[0].isMandatory);
        REQUIRE(tokens[0].nuggetOptions.size() == 1);
        REQUIRE(tokens[0].nuggetOptions[0] == "sure");
    }

    SECTION("Optional token") {
        SECTION("Without ID") {
            tokens = getStoryTokens("{maybe}");
            REQUIRE(tokens[0].id == "");
            REQUIRE(tokens[0].text == "{maybe}");
        }
        SECTION("With ID") {
            tokens = getStoryTokens("{maybe:xyz}");
            REQUIRE(tokens[0].id == "xyz");
            REQUIRE(tokens[0].text == "{maybe:xyz}");
        }
        SECTION("With text") {
            tokens = getStoryTokens("May {maybe:xyz} I say?");
            REQUIRE(tokens[0].id == "xyz");
            REQUIRE(tokens[0].text == "{maybe:xyz}");
        }
        REQUIRE(tokens.size() == 1);
        REQUIRE(!tokens[0].isMandatory);
        REQUIRE(tokens[0].nuggetOptions.size() == 1);
        REQUIRE(tokens[0].nuggetOptions[0] == "maybe");
    }

    SECTION("Complex string") {
        tokens = getStoryTokens(
                string("This is a {positive|subba:id2} part, (while) {THIS:is|was} just [negative] bla {bla}.") +
                " And this is {the absolute} [best|worst] [part} {of] [all[]]! Look, a [entity:id1x].");
        REQUIRE(tokens.size() == 5);

        int i = 0;
        REQUIRE(tokens[i].text == "{positive|subba:id2}");
        REQUIRE(tokens[i].id == "id2");
        REQUIRE(!tokens[i].isMandatory);
        REQUIRE(tokens[i].nuggetOptions.size() == 2);
        REQUIRE(tokens[i].nuggetOptions[0] == "positive");
        REQUIRE(tokens[i].nuggetOptions[1] == "subba");

        i++;
        REQUIRE(tokens[i].text == "[negative]");
        REQUIRE(tokens[i].id == "");
        REQUIRE(tokens[i].isMandatory);
        REQUIRE(tokens[i].nuggetOptions.size() == 1);
        REQUIRE(tokens[i].nuggetOptions[0] == "negative");

        i++;
        REQUIRE(tokens[i].text == "{bla}");
        REQUIRE(tokens[i].id == "");
        REQUIRE(!tokens[i].isMandatory);
        REQUIRE(tokens[i].nuggetOptions.size() == 1);
        REQUIRE(tokens[i].nuggetOptions[0] == "bla");

        i++;
        REQUIRE(tokens[i].text == "[best|worst]");
        REQUIRE(tokens[i].id == "");
        REQUIRE(tokens[i].isMandatory);
        REQUIRE(tokens[i].nuggetOptions.size() == 2);
        REQUIRE(tokens[i].nuggetOptions[0] == "best");
        REQUIRE(tokens[i].nuggetOptions[1] == "worst");

        i++;
        REQUIRE(tokens[i].text == "[entity:id1x]");
        REQUIRE(tokens[i].id == "id1x");
        REQUIRE(tokens[i].isMandatory);
        REQUIRE(tokens[i].nuggetOptions.size() == 1);
        REQUIRE(tokens[i].nuggetOptions[0] == "entity");
    }

}
