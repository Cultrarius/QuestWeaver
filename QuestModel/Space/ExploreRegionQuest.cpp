//
// Created by michael on 10.10.15.
//

#include "ExploreRegionQuest.h"

using namespace weave;
using namespace std;

ExploreRegionQuest::ExploreRegionQuest(ID id, QuestState state, const string &title, const string &description,
                                       ID location, ID sponsor) :
        Quest(id, state, title, description), targetLocation(location), sponsor(sponsor) {
}

shared_ptr<Quest> ExploreRegionQuest::setStateAndId(ID newId, QuestState newState) const {
    auto quest = new ExploreRegionQuest(newId, newState, GetTitle(), GetDescription(), targetLocation, sponsor);
    return shared_ptr<ExploreRegionQuest>(quest);
}

string ExploreRegionQuest::GetType() const {
    return "Space::ExploreRegion";
}

ExploreRegionQuest::ExploreRegionQuest(const string &title, const string &description, ID location, ID sponsor) :
        Quest(title, description), targetLocation(location), sponsor(sponsor) {
}
