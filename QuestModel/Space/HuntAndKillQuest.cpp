//
// Created by michael on 25.03.16.
//

#include "QuestModel/Space/HuntAndKillQuest.h"

using namespace weave;
using namespace std;

HuntAndKillQuest::HuntAndKillQuest(ID id, const string &title, const string &description,
                                   const std::string &story, ID targetShipId, ID solarSystemId, ID sponsorId) :
        Quest(id, title, description, story), targetShip(targetShipId), solarSystem(solarSystemId), sponsor(sponsor) {
}

string HuntAndKillQuest::GetType() const {
    return "Space::HuntAndKill";
}

HuntAndKillQuest::HuntAndKillQuest(const string &title, const string &description, const std::string &story,
                                   ID targetShipId, ID solarSystemId, ID sponsorId) :
        Quest(title, description, story), targetShip(targetShipId), solarSystem(solarSystemId), sponsor(sponsor) {
}

QuestTickResult HuntAndKillQuest::Tick(float, const WorldModel &worldModel) {
    // TODO: check if the ship is in the target system -> teleport it there if not

    int scanned = worldModel.GetMetaData(targetShip).GetValue(metaDataMarker);
    if (scanned >= 100) {
        //TODO improve sponsor relationship
        return QuestTickResult(QuestModelAction(QuestActionType::SUCCEED, GetId()));
    }
    return QuestTickResult(GetId());
}