//
// Created by michael on 12.10.15.
//

#include "catch.hpp"
#include "../Core/WeaverUtils.h"

using namespace weave;

TEST_CASE("Random Stream is tested", "[utils]") {
    RandomStream rs(42);
    REQUIRE(rs.GetIntInRange(1, 10) == 1);

    SECTION("reusing the stream") {
        REQUIRE(rs.GetIntInRange(1, 10) == 6);
    }

    SECTION("create random integers") {
        int a = rs.GetInt();
        int b = rs.GetInt();
        REQUIRE(a != b);
    }

    SECTION("reseeding the stream") {
        rs.Seed(42);
        REQUIRE(rs.GetIntInRange(1, 10) == 1);
    }

    SECTION("creating indices") {
        REQUIRE(rs.GetRandomIndex(1) == 0);
        REQUIRE(rs.GetRandomIndex(10) == 7);
    }
}
