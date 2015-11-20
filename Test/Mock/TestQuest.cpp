//
// Created by michael on 20.11.15.
//

#include "TestQuest.h"

using namespace weave;
using namespace std;

TestQuest::TestQuest(const string &title, const string &description) : Quest(title, description) {
}

TestQuest::TestQuest(ID id, QuestState state, const string &title, const string &description)
        : Quest(id, state, title, description) {
}

std::string TestQuest::GetType() const {
    return "Degub::TestQuest";
}

std::shared_ptr<Quest> TestQuest::setStateAndId(ID newId, QuestState newState) const {
    auto quest = new TestQuest(newId, newState, GetTitle(), GetDescription());
    return shared_ptr<TestQuest>(quest);
}
