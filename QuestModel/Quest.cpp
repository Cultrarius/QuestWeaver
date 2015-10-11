//
// Created by michael on 11.08.15.
//

#include "Quest.h"

QuestState Quest::getState() const {
    return state;
}

std::string Quest::getTitle() const {
    return title;
}

std::string Quest::getDescription() const {
    return description;
}

Quest::Quest(QuestState state, const std::string &title, const std::string &description) {
    this->state = state;
    this->title = title;
    this->description = description;
}
