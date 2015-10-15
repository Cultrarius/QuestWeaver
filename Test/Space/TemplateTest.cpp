//
// Created by michael on 12.10.15.
//

#include <string>
#include <memory>
#include "../catch.hpp"
#include "../../Core/WeaverUtils.h"
#include "../../Template/Space/SpaceQuestTemplateFactory.h"
#include "../../World/Space/SpaceWorldModel.h"

using namespace weave;
using namespace std;

TEST_CASE("Template factory", "[template]") {
    shared_ptr<RandomStream> rs = make_shared<RandomStream>(42);
    SpaceQuestTemplateFactory factory(rs);
    REQUIRE(factory.GetTemplateKeys().size() >= 1);

    SECTION("Checking the keys") {
        vector<string> keys = {"ExploreRegionQuest"};
        for (string key : keys) {
            bool hasKey = false;
            for (string templateKey : factory.GetTemplateKeys()) {
                if (key == templateKey) {
                    hasKey = true;
                    break;
                }
            }
            REQUIRE(hasKey);
        }
    }

    SECTION("Retrieving template for every key") {
        for (string templateKey : factory.GetTemplateKeys()) {
            auto temp = factory.CreateTemplate(templateKey);
            REQUIRE(temp.get() != nullptr);
        }
    }
}

TEST_CASE("Templates", "[template]") {
    shared_ptr<RandomStream> rs = make_shared<RandomStream>(42);
    SpaceQuestTemplateFactory factory(rs);

    SECTION("Checking properties size") {
        for (string templateKey : factory.GetTemplateKeys()) {
            auto tp = factory.CreateTemplate(templateKey);
            REQUIRE(tp->GetProperties().size() > 0);
        }
    }

    SECTION("Checking mandatory property exists") {
        for (string templateKey : factory.GetTemplateKeys()) {
            auto tp = factory.CreateTemplate(templateKey);
            bool hasMandatoryProperty = false;
            for (auto property : tp->GetProperties()) {
                if (property.IsMandatory()) {
                    hasMandatoryProperty = true;
                    break;
                }
            }
            INFO(templateKey);
            REQUIRE(hasMandatoryProperty);
        }
    }

    SECTION("Checking property names") {
        for (string templateKey : factory.GetTemplateKeys()) {
            auto tp = factory.CreateTemplate(templateKey);
            bool hasMandatoryProperty = false;
            for (auto property : tp->GetProperties()) {
                INFO(property.GetName());
                REQUIRE(!property.GetName().empty());
            }
        }
    }

    SECTION("Checking property candidates") {
        WorldModel *worldModel = new SpaceWorldModel(rs);
        for (string templateKey : factory.GetTemplateKeys()) {
            auto tp = factory.CreateTemplate(templateKey);
            bool hasMandatoryProperty = false;
            for (auto property : tp->GetProperties()) {
                auto candidates = tp->GetPropertyCandidates(property, *worldModel);
                INFO(templateKey + " / " + property.GetName());
                REQUIRE(candidates.size() > 0);
            }
        }
    }
}
