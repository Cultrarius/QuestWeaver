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
    dirs.modDirectory = "../Test/Resources/";
    shared_ptr<RandomStream> rs = make_shared<RandomStream>(42);
    TemplateEngine engine(rs, dirs, FormatterType::TEXT);
    QuestModel questModel;
    SpaceWorldModel worldModel(rs);
    WeaverGraph emptyGraph;
    vector<QuestPropertyValue> emptyValues;
    StoryWriter writer(rs, questModel, engine, worldModel, dirs);

    SECTION("Empty story test") {
        string story = writer.CreateStory(emptyGraph, emptyValues);
        REQUIRE("" == story);
    }

    SECTION("Valid nuggets") {
        shared_ptr<StoryTemplateFactory> templateFactory = make_shared<TestStoryTemplateFactory>("1");
        writer.RegisterTemplateFactory(templateFactory);
        string story = writer.CreateStory(emptyGraph, emptyValues);
        REQUIRE("" == story);
    }

    SECTION("Invalid nuggets - no key") {
        shared_ptr<StoryTemplateFactory> templateFactory = make_shared<TestStoryTemplateFactory>("2");
        writer.RegisterTemplateFactory(templateFactory);
        REQUIRE_THROWS_AS(writer.CreateStory(emptyGraph, emptyValues), ContractFailedException);
    }

    SECTION("Invalid nuggets - no required types") {
        shared_ptr<StoryTemplateFactory> templateFactory = make_shared<TestStoryTemplateFactory>("3");
        writer.RegisterTemplateFactory(templateFactory);
        REQUIRE_THROWS_AS(writer.CreateStory(emptyGraph, emptyValues), ContractFailedException);
    }

    SECTION("Invalid nuggets - no texts") {
        shared_ptr<StoryTemplateFactory> templateFactory = make_shared<TestStoryTemplateFactory>("3");
        writer.RegisterTemplateFactory(templateFactory);
        REQUIRE_THROWS_AS(writer.CreateStory(emptyGraph, emptyValues), ContractFailedException);
    }

    SECTION("Duplicate key") {
        shared_ptr<StoryTemplateFactory> templateFactory = make_shared<TestStoryTemplateFactory>("1");
        shared_ptr<StoryTemplateFactory> templateFactory2 = make_shared<TestStoryTemplateFactory>("5");
        writer.RegisterTemplateFactory(templateFactory);
        writer.RegisterTemplateFactory(templateFactory2);
        REQUIRE_THROWS_AS(writer.CreateStory(emptyGraph, emptyValues), ContractFailedException);
    }

    SECTION("Duplicate key, but same folder") {
        shared_ptr<StoryTemplateFactory> templateFactory = make_shared<TestStoryTemplateFactory>("1");
        shared_ptr<StoryTemplateFactory> templateFactory2 = make_shared<TestStoryTemplateFactory>("1");
        writer.RegisterTemplateFactory(templateFactory);
        writer.RegisterTemplateFactory(templateFactory2);
        string story = writer.CreateStory(emptyGraph, emptyValues);
        REQUIRE("" == story);
    }
}
