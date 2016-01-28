//
// Created by michael on 10.10.15.
//

#include <QuestModel/Space/ExploreRegionQuest.h>

using namespace weave;
using namespace std;

ExploreRegionQuest::ExploreRegionQuest(ID id, QuestState state, const string &title, const string &description,
                                       const std::string &story, ID location, ID sponsor) :
        Quest(id, state, title, description, story), targetLocation(location), sponsor(sponsor) {
}

shared_ptr<Quest> ExploreRegionQuest::setStateAndId(ID newId, QuestState newState) const {
    auto quest = new ExploreRegionQuest(newId, newState, GetTitle(), GetDescription(), GetStory(), targetLocation,
                                        sponsor);
    return shared_ptr<ExploreRegionQuest>(quest);
}

string ExploreRegionQuest::GetType() const {
    return "Space::ExploreRegion";
}

ExploreRegionQuest::ExploreRegionQuest(const string &title, const string &description, const std::string &story,
                                       ID location, ID sponsor) :
        Quest(title, description, story), targetLocation(location), sponsor(sponsor) {
}
