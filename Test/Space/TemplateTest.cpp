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

TEST_CASE("Template factory is tested", "[template]") {
    shared_ptr<RandomStream> rs = make_shared<RandomStream>(42);
    SpaceQuestTemplateFactory factory(rs);
    REQUIRE(factory.GetTemplateKeys().size() == 1);


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

    /*
    SECTION("create random integers") {
        int a = rs.GetInt();
        int b = rs.GetInt();
        REQUIRE(a != b);
    }

    SECTION("create integers with the same seed") {
        rs.Seed(1337);
        int a = rs.GetInt();
        rs.Seed(1337);
        int b = rs.GetInt();
        REQUIRE(a == b);
    }

    SECTION("reseeding the stream") {
        rs.Seed(42);
        REQUIRE(rs.GetIntInRange(1, 10) == 1);
    }

    SECTION("creating indices") {
        REQUIRE(rs.GetRandomIndex(1) == 0);
        REQUIRE(rs.GetRandomIndex(10) == 7);
    }*/
}
