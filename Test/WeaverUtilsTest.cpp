//
// Created by michael on 12.10.15.
//

#include <string>
#include "catch.hpp"
#include "../Core/WeaverUtils.h"

using namespace weave;
using namespace std;

TEST_CASE("Random Stream", "[utils]") {
    RandomStream rs(42);
    REQUIRE(rs.GetIntInRange(1, 10) == 4);

    SECTION("reusing the stream") {
        REQUIRE(rs.GetIntInRange(1, 10) == 8);
    }

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
        REQUIRE(rs.GetIntInRange(1, 10) == 4);
    }

    SECTION("creating indices") {
        REQUIRE(rs.GetRandomIndex(1) == 0);
        REQUIRE(rs.GetRandomIndex(10) == 9);
    }
}

TEST_CASE("Simple string substitution", "[utils]") {
    string s1 = "Hello World ASD FASD";
    string s3 = "Complex %bla with another %bling and %bla bla";
    string s4 = "Unicode! $&#+*^°ö%blaóäÜÖÄò";

    SECTION("Simple replace nothing") {
        string s2 = s1;
        replace(&s1, "XYZ", "Blargh");
        REQUIRE(s1 == s2);
    }

    SECTION("Replace single simple") {
        replace(&s1, "ASD", "XYZ");
        REQUIRE(s1 == "Hello World XYZ FASD");
    }

    SECTION("Replace single expand") {
        replace(&s1, "ASD", "this is a long string");
        REQUIRE(s1 == "Hello World this is a long string FASD");
    }

    SECTION("Replace single complex") {
        replace(&s3, "%bla", "string");
        REQUIRE(s3 == "Complex string with another %bling and %bla bla");
    }

    SECTION("Replace single unicode") {
        replace(&s4, "%bla", "string");
        REQUIRE(s4 == "Unicode! $&#+*^°östringóäÜÖÄò");
    }

    SECTION("Replace single unicode 2") {
        replace(&s4, "%bla", "èṕö");
        REQUIRE(s4 == "Unicode! $&#+*^°öèṕöóäÜÖÄò");
    }
}

TEST_CASE("Full string substitution", "[utils]") {
    string s1 = "Hello World ASD FASD";
    string s3 = "Complex %bla with another %bling and %bla bla";
    string s4 = "Unicode! $&#+*^°ö%blaóäÜÖÄò%bla";

    SECTION("Replace nothing") {
        string s2 = s1;
        replaceAll(&s1, "XYZ", "Blargh");
        REQUIRE(s1 == s2);
    }

    SECTION("Replace simple") {
        replaceAll(&s1, "ASD", "XYZ");
        REQUIRE(s1 == "Hello World XYZ FXYZ");
    }

    SECTION("Replace expand") {
        replaceAll(&s1, "ASD", "this is a long string");
        REQUIRE(s1 == "Hello World this is a long string Fthis is a long string");
    }

    SECTION("Replace complex") {
        replaceAll(&s3, "%bla", "string");
        REQUIRE(s3 == "Complex string with another %bling and string bla");
    }

    SECTION("Replace unicode") {
        replaceAll(&s4, "%bla", "string");
        REQUIRE(s4 == "Unicode! $&#+*^°östringóäÜÖÄòstring");
    }

    SECTION("Replace unicode 2") {
        replaceAll(&s4, "%bla", "èṕö");
        REQUIRE(s4 == "Unicode! $&#+*^°öèṕöóäÜÖÄòèṕö");
    }
}
