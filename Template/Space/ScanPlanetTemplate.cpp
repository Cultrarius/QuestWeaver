//
// Created by michael on 24.03.16.
//

#include "Template/Space/ScanPlanetTemplate.h"
#include <QuestModel/Space/ScanPlanetQuest.h>
#include <World/Space/Planet.h>
#include <World/Space/DeadCivilization.h>
#include <World/Space/SpaceAgent.h>
#include <World/Space/MetaDataMarkers.h>

using namespace std;
using namespace weave;

ScanPlanetTemplate::ScanPlanetTemplate(string title, vector<TemplateQuestProperty> properties,
                                       vector<TemplateQuestDescription> descriptions,
                                       FormatterType formatterType, int rarity)
        : QuestTemplate(title, properties, descriptions, formatterType, rarity, "ScanPlanetTemplate") {
}

vector<PropertyCandidate> ScanPlanetTemplate::GetPropertyCandidates(const TemplateQuestProperty &property,
                                                                    const WorldModel &worldModel) const {
    vector<PropertyCandidate> candidates;
    const SpaceWorldModel &spaceModel = (const SpaceWorldModel &) worldModel;
    if (property.GetName() == planetProperty) {
        gatherPlanetEntities(&candidates, spaceModel);
    } else if (property.GetName() == sponsorProperty) {
        gatherSponsorEntities(&candidates, spaceModel);
    } else if (property.GetName() == deadCivProperty) {
        gatherCivilizationEntities(&candidates, spaceModel);
    }
    return candidates;
}

shared_ptr<Quest> ScanPlanetTemplate::ToQuest(const vector<QuestPropertyValue> &questPropertyValues,
                                              const string &questStory) const {
    const string &description = getBestFittingDescription(questPropertyValues);
    const string &questTitle = getTitle(questPropertyValues);

    ID planetId = getEntityIdFromProperty(planetProperty, questPropertyValues);
    ID sponsor = getEntityIdFromProperty(sponsorProperty, questPropertyValues);
    return make_shared<ScanPlanetQuest>(questTitle, description, questStory, planetId, sponsor);
}

map<string, string> ScanPlanetTemplate::resolveAdditionalPlaceholders(
        QuestPropertyValue questPropertyValue) const noexcept {
    map<string, string> result;
    auto entity = questPropertyValue.GetValue();
    if (entity->GetType() == Planet::Type) {
        auto system = dynamic_pointer_cast<Planet>(entity);
        bool isMandatory = questPropertyValue.GetProperty().IsMandatory();
        result["location"] = QuestPropertyValue::GetValueString(system->Location, isMandatory, formatterType);
    }
    return result;
}

void ScanPlanetTemplate::gatherPlanetEntities(vector<PropertyCandidate> *candidates,
                                              const SpaceWorldModel &spaceModel) const {
    string scanPercent = ScanPlanetQuest::metaDataMarker;
    auto newEntityActions = spaceModel.CreateSolarSystem();
    vector<WorldModelAction> actions;
    shared_ptr<WorldEntity> newPlanet;
    for (auto action : newEntityActions) {
        if (action.GetEntity()->GetType() == Planet::Type) {
            newPlanet = action.GetEntity();
        }
        actions.push_back(action);
        MetaData metaData;
        metaData.SetValue(scanPercent, 0);
        metaData.SetValue(metaDataMarker, 1);  // so it does not get picked by another exploration quest
        WorldModelAction metaDataAction(WorldActionType::UPDATE, action.GetEntity(), metaData);
        actions.push_back(move(metaDataAction));
    }
    candidates->emplace_back(actions, newPlanet);

    // search for existing unexplored planets
    for (auto entity : spaceModel.GetEntitiesWithType(Planet::Type)) {
        auto entityData = spaceModel.GetMetaData(entity->GetId());
        if (!entityData.HasValue(metaDataMarker) && entityData.GetValue(scanPercent) == 0) {
            MetaData metaData;
            metaData.SetValue(scanPercent, 0);
            metaData.SetValue(metaDataMarker, 1);
            WorldModelAction modelAction(WorldActionType::UPDATE, entity, metaData);
            candidates->emplace_back(modelAction);
        }
    }
}

void ScanPlanetTemplate::gatherSponsorEntities(vector<PropertyCandidate> *candidates,
                                               const SpaceWorldModel &spaceModel) const {
    vector<WorldModelAction> actions;
    auto newEntityAction = spaceModel.CreateAgent();
    actions.push_back(newEntityAction);

    MetaData metaData;
    metaData.SetValue(MetaDataMarkers::RelationToPlayer, 50);
    WorldModelAction metaDataAction(WorldActionType::UPDATE, newEntityAction.GetEntity(), metaData);
    actions.push_back(move(metaDataAction));

    candidates->emplace_back(actions, newEntityAction.GetEntity());

    for (auto entity : spaceModel.GetEntitiesWithType(SpaceAgent::Type)) {
        auto entityData = spaceModel.GetMetaData(entity->GetId());
        if (entityData.GetValue(MetaDataMarkers::RelationToPlayer) >= 50) {
            WorldModelAction modelAction(WorldActionType::KEEP, entity);
            candidates->emplace_back(modelAction);
        }
    }
}

void ScanPlanetTemplate::gatherCivilizationEntities(vector<PropertyCandidate> *candidates,
                                                    const SpaceWorldModel &spaceModel) const {
    auto newEntityAction = spaceModel.CreateDeadCivilization();
    candidates->emplace_back(newEntityAction);

    for (auto entity : spaceModel.GetEntitiesWithType(DeadCivilization::Type)) {
        WorldModelAction modelAction(WorldActionType::KEEP, entity);
        candidates->emplace_back(modelAction);
    }
}
