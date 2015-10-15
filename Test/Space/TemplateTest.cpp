//
// Created by michael on 12.10.15.
//

#include <string>
#include <memory>
#include "../catch.hpp"
#include "../../Core/WeaverUtils.h"
#include "../../Template/Space/SpaceQuestTemplateFactory.h"

using namespace weave;
using namespace std;

TEST_CASE("Template factory", "[template]") {
    shared_ptr<RandomStream> rs = make_shared<RandomStream>(42);
    SpaceQuestTemplateFactory factory(rs);
    REQUIRE(factory.GetTemplateKeys().size() >= 1);


    SECTION("checking the keys") {
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
