//
// Created by michael on 12.10.15.
//

#include "catch.hpp"
#include "Mock/TestStoryTemplateFactory.h"
#include "Mock/TestEntity.h"
#include <Story/StoryWriter.h>
#include <World/Space/SpaceWorldModel.h>

using namespace weave;
using namespace std;

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
        auto result = writer.CreateStory(graph, values);
        REQUIRE("" == result.story);
    }

    SECTION("Valid nuggets") {
        writer.RegisterTemplateFactory(make_unique<TestStoryTemplateFactory>("1"));
        auto result = writer.CreateStory(graph, values);
        REQUIRE("" == result.story);
    }

    SECTION("Invalid nuggets - no key") {
        writer.RegisterTemplateFactory(make_unique<TestStoryTemplateFactory>("2"));
        REQUIRE_THROWS_AS(writer.CreateStory(graph, values), ContractFailedException);
    }

    SECTION("Invalid nuggets - no required types") {
        writer.RegisterTemplateFactory(make_unique<TestStoryTemplateFactory>("3"));
        REQUIRE_THROWS_AS(writer.CreateStory(graph, values), ContractFailedException);
    }

    SECTION("Invalid nuggets - no texts") {
        writer.RegisterTemplateFactory(make_unique<TestStoryTemplateFactory>("3"));
        REQUIRE_THROWS_AS(writer.CreateStory(graph, values), ContractFailedException);
    }

    SECTION("Invalid nuggets - no outer json array") {
        writer.RegisterTemplateFactory(make_unique<TestStoryTemplateFactory>("6"));
        REQUIRE_THROWS_AS(writer.CreateStory(graph, values), ContractFailedException);
    }

    SECTION("Invalid nuggets - no inner json object") {
        writer.RegisterTemplateFactory(make_unique<TestStoryTemplateFactory>("7"));
        REQUIRE_THROWS_AS(writer.CreateStory(graph, values), ContractFailedException);
    }

    SECTION("Duplicate key") {
        writer.RegisterTemplateFactory(make_unique<TestStoryTemplateFactory>("1"));
        writer.RegisterTemplateFactory(make_unique<TestStoryTemplateFactory>("5"));
        REQUIRE_THROWS_AS(writer.CreateStory(graph, values), ContractFailedException);
    }

    SECTION("Duplicate key, but same folder") {
        writer.RegisterTemplateFactory(make_unique<TestStoryTemplateFactory>("1"));
        writer.RegisterTemplateFactory(make_unique<TestStoryTemplateFactory>("1"));
        auto result = writer.CreateStory(graph, values);
        REQUIRE("" == result.story);
    }

    SECTION("Adding agent entity") {

        // create an agent in the world model
        auto agent = worldModel.CreateAgent();
        TemplateQuestProperty templateProperty(true, "superhero");
        values.push_back(QuestPropertyValue(templateProperty, agent));
        set<string> requiredTypes = {"agent"};

        // create a fitting story template factory
        writer.RegisterTemplateFactory(make_unique<TestStoryTemplateFactory>("1", requiredTypes));

        // create a fitting graph node
        graph.CreateNodeGroup("test", true);
        graph.AddNode(Node("test", agent->GetId()));
        graph.Finalize();

        // run it through the story writer
        auto result = writer.CreateStory(graph, values);
        REQUIRE(result.story == "");
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
        writer.RegisterTemplateFactory(make_unique<TestStoryTemplateFactory>("8", "storyLines.st"));
        auto result = writer.CreateStory(graph, values, "simpleLine");
        REQUIRE(result.story == "A. B. C");
    }

    SECTION("Entity line test") {
        writer.RegisterTemplateFactory(make_unique<TestStoryTemplateFactory>("8", "storyLines.st"));
        auto result = writer.CreateStory(graph, values, "entityLine");
        REQUIRE(result.story == "I wish me a TestEntity to play with.");
    }

    SECTION("String only line test") {
        writer.RegisterTemplateFactory(make_unique<TestStoryTemplateFactory>("8", "storyLines.st"));
        auto result = writer.CreateStory(graph, values, "stringOnlyLines");
        REQUIRE(result.story == "This is great! I really love this.");
    }

    SECTION("Partial lines test") {
        writer.RegisterTemplateFactory(make_unique<TestStoryTemplateFactory>("8", "storyLines.st"));
        auto result = writer.CreateStory(graph, values, "partialLines");
        REQUIRE(result.story == "This is the result of some partial lines.");
    }

    SECTION("Unknown nugget") {
        writer.RegisterTemplateFactory(make_unique<TestStoryTemplateFactory>("8", "storyLines.st"));
        auto result = writer.CreateStory(graph, values, "unknownNugget");
        REQUIRE(result.story == "");
    }

    SECTION("Nugget IDs mismatch") {
        writer.RegisterTemplateFactory(make_unique<TestStoryTemplateFactory>("8", "storyLines.st"));
        REQUIRE_THROWS_AS(writer.CreateStory(graph, values, "wrongNuggetIds"), ContractFailedException);
    }

    SECTION("Nugget content mismatch") {
        writer.RegisterTemplateFactory(make_unique<TestStoryTemplateFactory>("8", "storyLines.st"));
        REQUIRE_THROWS_AS(writer.CreateStory(graph, values, "wrongNuggetContent"), ContractFailedException);
    }

    SECTION("Directory change") {
        writer.RegisterTemplateFactory(make_unique<TestStoryTemplateFactory>("8", "storyLines.st"));
        auto result = writer.CreateStory(graph, values, "entityLine");
        REQUIRE(result.story == "I wish me a TestEntity to play with.");
        writer.ChangeDirectories(dirs);
        engine.ChangeDirectories(dirs);
        result = writer.CreateStory(graph, values, "entityLine");
        REQUIRE(result.story == "I wish me a TestEntity to play with.");

    }

    SECTION("Invalid entity IDs") {
        auto factory = make_unique<TestStoryTemplateFactory>("8", "storyLines.st");
        factory->TemplatesReturnInvalidIDs = true;
        writer.RegisterTemplateFactory(move(factory));
        REQUIRE_THROWS_AS(writer.CreateStory(graph, values, "entityLine"), ContractFailedException);
    }

    SECTION("Broken template") {
        SECTION("No array as outer struct") {
            writer.RegisterTemplateFactory(make_unique<TestStoryTemplateFactory>("8", "brokenNoArray.st"));
            REQUIRE_THROWS_AS(writer.CreateStory(graph, values, "simpleLine"), ContractFailedException);
        }

        SECTION("No Key for story") {
            writer.RegisterTemplateFactory(make_unique<TestStoryTemplateFactory>("8", "brokenNoKey.st"));
            REQUIRE_THROWS_AS(writer.CreateStory(graph, values, "simpleLine"), ContractFailedException);
        }
    }
}