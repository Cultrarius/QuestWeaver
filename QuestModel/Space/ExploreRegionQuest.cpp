//
// Created by michael on 10.10.15.
//

#include <QuestModel/Space/ExploreRegionQuest.h>

using namespace weave;
using namespace std;

ExploreRegionQuest::ExploreRegionQuest(ID id, const string &title, const string &description,
                                       const std::string &story, ID location, ID sponsor) :
        Quest(id, title, description, story), targetLocation(location), sponsor(sponsor) {
}

string ExploreRegionQuest::GetType() const {
    return "Space::ExploreRegion";
}

ExploreRegionQuest::ExploreRegionQuest(const string &title, const string &description, const std::string &story,
                                       ID location, ID sponsor) :
        Quest(title, description, story), targetLocation(location), sponsor(sponsor) {
}
