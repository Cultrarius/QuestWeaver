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

ExploreRegionQuest::ExploreRegionQuest(ID id, QuestState state, const std::string &title,
                                       const std::string &description) : Quest(id, state, title, description) {
}

std::shared_ptr<Quest> ExploreRegionQuest::setStateAndId(ID newId, QuestState newState) const {
    auto quest = new ExploreRegionQuest(newId, newState, GetTitle(), GetDescription());
    return shared_ptr<ExploreRegionQuest>(quest);
}

string ExploreRegionQuest::GetType() const {
    return "Space::ExploreRegion";
}
