//
// Created by michael on 20.11.15.
//

#include "TestQuest.h"

using namespace weave;
using namespace std;

TestQuest::TestQuest(const string &title, const string &description) : Quest(title, description) {
}

TestQuest::TestQuest(ID id, const string &title, const string &description)
        : Quest(id, title, description, "Once upon a time...") {
}

std::string TestQuest::GetType() const {
    return "Debug::TestQuest";
}
