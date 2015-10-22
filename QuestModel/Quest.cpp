//
// Created by michael on 11.08.15.
//

#include "Quest.h"

using namespace weave;

QuestState Quest::GetState() const {
    return state;
}

std::string Quest::GetTitle() const {
    return title;
}

std::string Quest::GetDescription() const {
    return description;
}

Quest::Quest(const std::string &title, const std::string &description) :
        Quest(NoID, QuestState::Proposed, title, description) {
}

Quest::Quest(ID newId, QuestState state, const std::string &title, const std::string &description) :
        id(newId), state(state), title(title), description(description) {
}


ID Quest::GetId() const {
    return id;
}

std::vector<ModelAction> Quest::Tick(float delta) {
    return std::vector<ModelAction>();
}
