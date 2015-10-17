//
// Created by michael on 10.10.15.
//

#include "ExploreRegionQuest.h"

using namespace weave;
using namespace std;

ExploreRegionQuest::ExploreRegionQuest(const std::string &title,
                                       const std::string &description) :
        Quest(title, description) {
}

std::shared_ptr<Quest> ExploreRegionQuest::setState(QuestState newState) const {
    auto newQuest = make_shared<ExploreRegionQuest>(GetTitle(), GetDescription());
    return newQuest;
}
