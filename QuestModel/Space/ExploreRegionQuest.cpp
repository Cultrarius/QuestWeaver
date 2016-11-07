//
// Created by michael on 10.10.15.
//

#include <QuestModel/Space/ExploreRegionQuest.h>
#include <World/Space/MetaDataMarkers.h>

using namespace weave;
using namespace std;

const string ExploreRegionQuest::metaDataMarker = "exploredPercent";

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

QuestTickResult ExploreRegionQuest::Tick(float, const WorldModel &worldModel) {
    int explored = worldModel.GetMetaData(targetLocation).GetValue(metaDataMarker);
    if (explored >= 100) {
        string marker = MetaDataMarkers::RelationToPlayer;
        auto updater = [](int oldValue) {
            int newRelation = relationAdd + oldValue;
            return newRelation > 100 ? 100 : newRelation;
        };
        WorldModelAction improveRelation = worldModel.ChangeMetaData(sponsor, marker, updater);
        return QuestTickResult({improveRelation}, QuestModelAction(QuestActionType::SUCCEED, GetId()));
    }
    return QuestTickResult(GetId());
}
