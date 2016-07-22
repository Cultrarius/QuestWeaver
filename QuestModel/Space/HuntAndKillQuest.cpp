//
// Created by michael on 25.03.16.
//

#include <World/Space/MetaDataMarkers.h>
#include "QuestModel/Space/HuntAndKillQuest.h"

using namespace weave;
using namespace std;

HuntAndKillQuest::HuntAndKillQuest(ID id, const string &title, const string &description,
                                   const std::string &story, ID targetShipId, ID solarSystemId, ID sponsorId) :
        Quest(id, title, description, story), targetShip(targetShipId), solarSystem(solarSystemId), sponsor(sponsorId) {
}

string HuntAndKillQuest::GetType() const {
    return "Space::HuntAndKill";
}

HuntAndKillQuest::HuntAndKillQuest(const string &title, const string &description, const std::string &story,
                                   ID targetShipId, ID solarSystemId, ID sponsorId) :
        Quest(title, description, story), targetShip(targetShipId), solarSystem(solarSystemId), sponsor(sponsorId) {
}

QuestTickResult HuntAndKillQuest::Tick(float, const WorldModel &worldModel) {
    vector<WorldModelAction> worldChanges;
    auto targetData = worldModel.GetMetaData(targetShip);
    if (targetData.GetValue(MetaDataMarkers::Destroyed)) {
        if (sponsor) {
            MetaData updated;
            int newRelation = relationAdd + worldModel.GetMetaData(sponsor).GetValue(MetaDataMarkers::RelationToPlayer);
            updated.SetValue(MetaDataMarkers::RelationToPlayer, newRelation > 100 ? 100 : newRelation);
            worldChanges.emplace_back(WorldActionType::UPDATE, worldModel.GetEntityById(sponsor), updated);
        }
        return QuestTickResult(worldChanges, QuestModelAction(QuestActionType::SUCCEED, GetId()));
    }

    // move target to the quest solar system if necessary
    if (worldModel.GetEntityById(targetShip) &&
        (ID) targetData.GetValue(MetaDataMarkers::CurrentLocation) != solarSystem) {
        MetaData updated;
        updated.SetValue(MetaDataMarkers::CurrentLocation, solarSystem);
        worldChanges.emplace_back(WorldActionType::UPDATE, worldModel.GetEntityById(targetShip), updated);
    }
    return QuestTickResult(GetId(), worldChanges);
}
