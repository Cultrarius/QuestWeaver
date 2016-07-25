//
// Created by michael on 24.03.16.
//

#include "Template/Space/HuntAndKillTemplate.h"
#include <QuestModel/Space/HuntAndKillQuest.h>
#include <World/Space/SpaceAgent.h>
#include <World/Space/SpaceShip.h>
#include <World/Space/MetaDataMarkers.h>

using namespace std;
using namespace weave;

HuntAndKillTemplate::HuntAndKillTemplate(string title, vector<TemplateQuestProperty> properties,
                                         vector<TemplateQuestDescription> descriptions,
                                         FormatterType formatterType, int rarity)
        : QuestTemplate(title, properties, descriptions, formatterType, rarity) {
}

vector<PropertyCandidate> HuntAndKillTemplate::GetPropertyCandidates(const TemplateQuestProperty &property,
                                                                     const WorldModel &worldModel) const {
    vector<PropertyCandidate> candidates;
    const SpaceWorldModel &spaceModel = (const SpaceWorldModel &) worldModel;
    if (property.GetName() == solarSystemProperty) {
        gatherSolarSystemEntities(&candidates, spaceModel);
    } else if (property.GetName() == targetShipProperty) {
        gatherTargetEntities(&candidates, spaceModel);
    } else if (property.GetName() == sponsorProperty) {
        gatherSponsorEntities(&candidates, spaceModel);
    }
    return candidates;
}

shared_ptr<Quest> HuntAndKillTemplate::ToQuest(const vector<QuestPropertyValue> &questPropertyValues,
                                               const string &questStory) const {
    const string &description = getBestFittingDescription(questPropertyValues);
    const string &questTitle = getTitle(questPropertyValues);

    ID solarSystemID = getEntityIdFromProperty(solarSystemProperty, questPropertyValues);
    ID targetID = getEntityIdFromProperty(targetShipProperty, questPropertyValues);
    ID sponsorID = getEntityIdFromProperty(sponsorProperty, questPropertyValues);
    return make_shared<HuntAndKillQuest>(questTitle, description, questStory, solarSystemID, targetID, sponsorID);
}

map<string, string> HuntAndKillTemplate::resolveAdditionalPlaceholders(
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

void HuntAndKillTemplate::gatherSponsorEntities(vector<PropertyCandidate> *candidates,
                                                const SpaceWorldModel &spaceModel) const {
    vector<WorldModelAction> actions;

    // create new sponsor
    auto newEntityAction = spaceModel.CreateAgent(NameType::LIGHT_PERSON);
    MetaData metaData;
    metaData.SetValue(MetaDataMarkers::RelationToPlayer, 65);
    WorldModelAction metaDataAction(WorldActionType::CREATE, newEntityAction.GetEntity(), metaData);
    actions.push_back(move(metaDataAction));
    candidates->emplace_back(actions, newEntityAction.GetEntity());

    // gather existing sponsors
    for (auto entity : spaceModel.GetEntitiesWithType(SpaceAgent::Type)) {
        auto entityData = spaceModel.GetMetaData(entity->GetId());
        if (entityData.GetValue(MetaDataMarkers::RelationToPlayer) >= 65) {
            WorldModelAction modelAction(WorldActionType::KEEP, entity);
            candidates->emplace_back(modelAction);
        }
    }
}

void HuntAndKillTemplate::gatherSolarSystemEntities(std::vector<PropertyCandidate> *candidates,
                                                    const SpaceWorldModel &spaceModel) const {
    auto newEntityActions = spaceModel.CreateSolarSystem();
    shared_ptr<WorldEntity> newSolarSystem;
    for (auto action : newEntityActions) {
        if (action.GetEntity()->GetType() == SolarSystem::Type) {
            newSolarSystem = action.GetEntity();
        }
    }
    candidates->emplace_back(newEntityActions, newSolarSystem);

    // gather existing solar systems
    for (auto entity : spaceModel.GetEntitiesWithType(SolarSystem::Type)) {
        WorldModelAction modelAction(WorldActionType::KEEP, entity);
        candidates->emplace_back(modelAction);
    }
}

void HuntAndKillTemplate::gatherTargetEntities(std::vector<PropertyCandidate> *candidates,
                                               const SpaceWorldModel &spaceModel) const {
    vector<WorldModelAction> newShipActions;
    // find or create owner for new ship
    shared_ptr<SpaceAgent> newOwner;
    for (auto entity : spaceModel.GetEntitiesWithType(SpaceAgent::Type)) {
        auto entityData = spaceModel.GetMetaData(entity->GetId());
        if (entityData.GetValue(MetaDataMarkers::RelationToPlayer) <= 50) {
            WorldModelAction modelAction(WorldActionType::KEEP, entity);
            newShipActions.push_back(modelAction);
            newOwner = dynamic_pointer_cast<SpaceAgent>(entity);
            break;
        }
    }
    if (!newOwner) {
        auto newEntityAction = spaceModel.CreateAgent(NameType::DARK_PERSON);
        MetaData metaData;
        metaData.SetValue(MetaDataMarkers::RelationToPlayer, 35);
        WorldModelAction metaDataAction(WorldActionType::CREATE, newEntityAction.GetEntity(), metaData);
        newShipActions.push_back(move(metaDataAction));

        newOwner = dynamic_pointer_cast<SpaceAgent>(newEntityAction.GetEntity());
    }
    auto newShipAction = spaceModel.CreateSpaceShip(newOwner, NameType::DARK_THING);
    newShipActions.push_back(newShipAction);
    candidates->emplace_back(newShipActions, newShipAction.GetEntity());

    // gather existing spaceships
    for (auto entity : spaceModel.GetEntitiesWithType(SpaceShip::Type)) {
        auto ship = dynamic_pointer_cast<SpaceShip>(entity);
        auto shipData = spaceModel.GetMetaData(ship->GetId());
        auto ownerData = spaceModel.GetMetaData(ship->Owner->GetId());
        if (!shipData.HasValue(metaDataMarker) && ownerData.GetValue(MetaDataMarkers::RelationToPlayer) <= 50) {
            MetaData metaData;
            metaData.SetValue(metaDataMarker, 1);
            WorldModelAction modelAction(WorldActionType::UPDATE, entity, metaData);
            candidates->emplace_back(modelAction);
        }
    }
}
