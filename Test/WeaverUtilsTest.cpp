//
// Created by michael on 12.10.15.
//

#include <string>
#include <Core/WeaverTypes.h>
#include <Core/WeaverUtils.h>
#include <Core/NameGen/TokenNameGenerator.h>
#include <World/Space/SpaceNameGenerator.h>
#include "catch.hpp"

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

    SECTION("creating normalized ints") {
        SECTION("Zeros") {
            for (int i = 0; i < 1000; i++) {
                int k = rs.GetNormalIntInRange(0, 0);
                REQUIRE(k == 0);
            }
        }

        SECTION("Start=End") {
            for (int i = 0; i < 1000; i++) {
                int k = rs.GetNormalIntInRange(-42, -42);
                REQUIRE(k == -42);
            }
        }

        SECTION("In Range") {
            map<int, int> seenNumbers;
            for (int k = 0; k < 10000; k++) {
                int i = rs.GetNormalIntInRange(-3, 17);
                seenNumbers[i]++;
            }

            for (int i = -3; i <= 17; i++) {
                REQUIRE(seenNumbers[i] > 0);
            }
        }

        SECTION("Small Range") {
            map<int, int> seenNumbers;
            for (int k = 0; k < 1000; k++) {
                int i = rs.GetNormalIntInRange(1, 5);
                seenNumbers[i]++;
            }

            REQUIRE(seenNumbers[2] > seenNumbers[1]);
            REQUIRE(seenNumbers[3] > seenNumbers[2]);
            REQUIRE(seenNumbers[3] > seenNumbers[4]);
            REQUIRE(seenNumbers[4] > seenNumbers[5]);
        }
    }
}

TEST_CASE("Name Generator", "[utils]") {
    auto rs = make_shared<RandomStream>();

    SECTION("reseeding the stream") {
        rs->Seed(7);
        TokenNameGenerator nameGenerator("iV");
        string name = nameGenerator.toString(rs);
        REQUIRE(name != "");
        for (int i = 0; i < 20; i++) {
            rs->Seed(7);
            string newName = nameGenerator.toString(rs);
            REQUIRE(name == newName);
        }
    }

    SECTION("random stream") {
        set<string> seenNames;
        rs->Seed(7);
        TokenNameGenerator nameGenerator("!<c(o|u)(b|k|kk|ck|gr|x|z|zz|s|ss)<(a|u|o)C|('|-)!C>v<|x|z|b|r|k>>");
        for (int i = 0; i < 2000; i++) {
            string name = nameGenerator.toString(rs);
            REQUIRE(name != "");
            REQUIRE(seenNames.count(name) == 0);
            seenNames.insert(name);
        }
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

    SECTION("Replace from empty") {
        replace(&s1, "", "this is a long string");
        REQUIRE(s1 == "this is a long stringHello World ASD FASD");
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

    SECTION("Replace from empty") {
        replaceAll(&s1, "", "this is a long string");
        REQUIRE(s1 == "Hello World ASD FASD");
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

TEST_CASE("HTML tag enclosure", "[utils]") {
    string s1 = "Hello World";
    string s2 = "Unicode! $&#+*^°ö%blaóäÜÖÄò%bla";
    vector<string> classOne = {"format"};
    vector<string> classTwo = {"overkill", "xTend"};

    SECTION("Empty all") {
        REQUIRE("" == htmlEncloseWithTag("", ""));
    }

    SECTION("Empty tag") {
        REQUIRE(s1 == htmlEncloseWithTag(s1, ""));
    }

    SECTION("Empty tag with class") {
        REQUIRE(s1 == htmlEncloseWithTag(s1, "", classOne));
    }

    SECTION("simple tag") {
        REQUIRE("<div>Hello World</div>" == htmlEncloseWithTag(s1, "div"));
    }

    SECTION("simple tag on unicode") {
        REQUIRE("<span>Unicode! $&#+*^°ö%blaóäÜÖÄò%bla</span>" == htmlEncloseWithTag(s2, "span"));
    }

    SECTION("simple tag with class") {
        REQUIRE("<div class=\"format\">Hello World</div>" == htmlEncloseWithTag(s1, "div", classOne));
    }

    SECTION("simple tag with class") {
        REQUIRE("<div class=\"whee\">asd</div>" == htmlEncloseWithTag("asd", "div", "whee"));
    }

    SECTION("simple tag with classes") {
        REQUIRE("<div class=\"overkill xTend\">Hello World</div>" == htmlEncloseWithTag(s1, "div", classTwo));
    }
}

TEST_CASE("Json Reader", "[utils]") {
    SECTION("Read broken JSON") {
        Directories testDirs;
        testDirs.templateDirectory = "Test/Resources/";
        testDirs.modDirectory = "../Test/Resources/";
        REQUIRE_THROWS_AS(readJsonFromFile("broken.qt", testDirs), ContractFailedException);
    }
}

TEST_CASE("StringTokenizer", "[utils]") {
    SECTION("Read test tokens") {
        string text = "ABC [def |g] {hij| klmn op | ööö} {{hallo}} {|} {test||123} uvw";
        cout << text << endl;
        auto tokens = tokenizeText(text);
        for (auto token : tokens) {
            cout << token.isMandatory << ": <" << token.rawTokenText << ">\n";
            for (auto choice : token.content) {
                cout << "  -> _" << choice << "_\n";
            }
        }
        cout << "----------\n";
        tokens = tokenizeText("{def}");
        for (auto token : tokens) {
            cout << token.isMandatory << ": <" << token.rawTokenText << ">\n";
        }
    }
}
