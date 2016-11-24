//
// Created by michael on 25.03.16.
//

#include <World/Space/MetaDataMarkers.h>
#include "QuestModel/Space/ScanPlanetQuest.h"

using namespace weave;
using namespace std;

const string ScanPlanetQuest::metaDataMarker = "scanPercent";

ScanPlanetQuest::ScanPlanetQuest(ID id, const string &title, const string &description,
                                 const std::string &story, ID planetId, ID sponsor) :
        Quest(id, title, description, story), targetPlanet(planetId), sponsor(sponsor) {
}

string ScanPlanetQuest::GetType() const {
    return "Space::ScanPlanet";
}

ScanPlanetQuest::ScanPlanetQuest(const string &title, const string &description, const std::string &story,
                                 ID planetId, ID sponsor) :
        Quest(title, description, story), targetPlanet(planetId), sponsor(sponsor) {
}

QuestTickResult ScanPlanetQuest::Tick(float, const WorldModel &worldModel) {
    int scanned = worldModel.GetMetaData(targetPlanet).GetValue(metaDataMarker);
    if (scanned >= 100) {
        vector<WorldModelAction> worldChanges;
        if (sponsor) {
            auto updater = [](int oldVal) {
                int newRelation = relationAdd + oldVal;
                return newRelation > 100 ? 100 : newRelation;
            };
            WorldModelAction updated = worldModel.ChangeMetaData(sponsor, MetaDataMarkers::RelationToPlayer, updater);
            worldChanges.push_back(updated);
        }
        return QuestTickResult(worldChanges, QuestModelAction(QuestActionType::SUCCEED, GetId()));
    }
    return QuestTickResult(GetId());
}