//
// Created by michael on 12.10.15.
//

#include "catch.hpp"
#include "Mock/TestStoryTemplateFactory.h"
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
        string story = writer.CreateStory(graph, values);
        REQUIRE("" == story);
    }

    SECTION("Valid nuggets") {
        shared_ptr<StoryTemplateFactory> templateFactory = make_shared<TestStoryTemplateFactory>("1");
        writer.RegisterTemplateFactory(templateFactory);
        string story = writer.CreateStory(graph, values);
        REQUIRE("" == story);
    }

    SECTION("Invalid nuggets - no key") {
        shared_ptr<StoryTemplateFactory> templateFactory = make_shared<TestStoryTemplateFactory>("2");
        writer.RegisterTemplateFactory(templateFactory);
        REQUIRE_THROWS_AS(writer.CreateStory(graph, values), ContractFailedException);
    }

    SECTION("Invalid nuggets - no required types") {
        shared_ptr<StoryTemplateFactory> templateFactory = make_shared<TestStoryTemplateFactory>("3");
        writer.RegisterTemplateFactory(templateFactory);
        REQUIRE_THROWS_AS(writer.CreateStory(graph, values), ContractFailedException);
    }

    SECTION("Invalid nuggets - no texts") {
        shared_ptr<StoryTemplateFactory> templateFactory = make_shared<TestStoryTemplateFactory>("3");
        writer.RegisterTemplateFactory(templateFactory);
        REQUIRE_THROWS_AS(writer.CreateStory(graph, values), ContractFailedException);
    }

    SECTION("Duplicate key") {
        shared_ptr<StoryTemplateFactory> templateFactory = make_shared<TestStoryTemplateFactory>("1");
        shared_ptr<StoryTemplateFactory> templateFactory2 = make_shared<TestStoryTemplateFactory>("5");
        writer.RegisterTemplateFactory(templateFactory);
        writer.RegisterTemplateFactory(templateFactory2);
        REQUIRE_THROWS_AS(writer.CreateStory(graph, values), ContractFailedException);
    }

    SECTION("Duplicate key, but same folder") {
        shared_ptr<StoryTemplateFactory> templateFactory = make_shared<TestStoryTemplateFactory>("1");
        shared_ptr<StoryTemplateFactory> templateFactory2 = make_shared<TestStoryTemplateFactory>("1");
        writer.RegisterTemplateFactory(templateFactory);
        writer.RegisterTemplateFactory(templateFactory2);
        string story = writer.CreateStory(graph, values);
        REQUIRE("" == story);
    }

    SECTION("Adding agent entity") {

        // create an agent in the world model
        auto agent = worldModel.CreateAgent();
        TemplateQuestProperty templateProperty(true, "superhero");
        values.push_back(QuestPropertyValue(templateProperty, agent));
        vector<string> requiredTypes = {"agent"};

        // create a fitting story template factory
        shared_ptr<StoryTemplateFactory> templateFactory = make_shared<TestStoryTemplateFactory>("1", requiredTypes);
        writer.RegisterTemplateFactory(templateFactory);

        // create a fitting graph node
        graph.CreateNodeGroup("test", true);
        graph.AddNode(Node("test", agent->GetId()));
        graph.Finalize();

        // run it through the story writer
        string story = writer.CreateStory(graph, values);
        REQUIRE(!story.empty());
    }
}
