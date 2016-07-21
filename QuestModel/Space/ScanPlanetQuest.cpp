//
// Created by michael on 25.03.16.
//

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
        //TODO improve sponsor relationship
        return QuestTickResult(QuestModelAction(QuestActionType::SUCCEED, GetId()));
    }
    return QuestTickResult(GetId());
}