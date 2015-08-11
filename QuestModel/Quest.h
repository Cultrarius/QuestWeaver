//
// Created by michael on 11.08.15.
//

#pragma once

#include <string>

enum QuestState {
    Active, Proposed, Failed, Success
};

class Quest {
public:
    Quest(QuestState state,
          std::string title,
          std::string description);

    QuestState getState() const;

    std::string getTitle() const;

    std::string getDescription() const;

private:
    QuestState state;
    std::string title;
    std::string description;
};
