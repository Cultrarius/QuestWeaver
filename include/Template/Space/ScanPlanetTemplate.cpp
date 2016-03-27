//
// Created by michael on 24.03.16.
//

#include "ScanPlanetTemplate.h"
#include <QuestModel/Space/ScanPlanetQuest.h>
#include <World/Space/Planet.h>

using namespace std;
using namespace weave;

ScanPlanetTemplate::ScanPlanetTemplate(string title, vector<TemplateQuestProperty> properties,
                                       vector<TemplateQuestDescription> descriptions,
                                       FormatterType formatterType)
        : QuestTemplate(title, properties, descriptions, formatterType) {
}

vector<PropertyCandidate> ScanPlanetTemplate::GetPropertyCandidates(const TemplateQuestProperty &property,
                                                                    const WorldModel &worldModel) const {
    vector<PropertyCandidate> candidates;
    const SpaceWorldModel &spaceModel = (const SpaceWorldModel &) worldModel;
    if (property.GetName() == "planet") {
        gatherPlanetEntities(&candidates, spaceModel);
    } else if (property.GetName() == "sponsor") {
        gatherSponsorEntities(&candidates, spaceModel);
    } else if (property.GetName() == "deadCivilization") {
        gatherCivilizationEntities(&candidates, spaceModel);
    }
    return candidates;
}

shared_ptr<Quest> ScanPlanetTemplate::ToQuest(const vector<QuestPropertyValue> &questPropertyValues,
                                              const string &questStory) const {
    const string &description = getBestFittingDescription(questPropertyValues);
    const string &questTitle = getTitle(questPropertyValues);

    ID planetId = getEntityIdFromProperty("planet", questPropertyValues);
    ID sponsor = getEntityIdFromProperty("sponsor", questPropertyValues);
    return make_shared<ScanPlanetQuest>(questTitle, description, questStory, planetId, sponsor);
}

map<string, string> ScanPlanetTemplate::resolveAdditionalPlaceholders(
        QuestPropertyValue questPropertyValue) const noexcept {
    map<string, string> result;
    auto entity = questPropertyValue.GetValue();
    if (entity->GetType() == "planet") {
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
    shared_ptr<WorldEntity> newSolarSystem;
    for (auto action : newEntityActions) {
        if (action.GetEntity()->GetType() == "solarSystem") {
            newSolarSystem = action.GetEntity();
        }
        actions.push_back(action);
        MetaData metaData;
        metaData.SetValue(scanPercent, 0);
        metaData.SetValue(metaDataMarker, 1);  // so it does not get picked by another exploration quest
        WorldModelAction metaDataAction(WorldActionType::UPDATE, action.GetEntity(), metaData);
        actions.push_back(move(metaDataAction));
    }
    candidates->push_back(PropertyCandidate(actions, newSolarSystem));

    // search for existing unexplored planets
    for (auto entity : spaceModel.GetEntities()) {
        if (entity->GetType() == "planet") {
            auto entityData = spaceModel.GetMetaData(entity->GetId());
            if (!entityData.HasValue(metaDataMarker) && entityData.GetValue(scanPercent) == 0) {
                MetaData metaData;
                metaData.SetValue(scanPercent, 0);
                metaData.SetValue(metaDataMarker, 1);
                WorldModelAction modelAction(WorldActionType::UPDATE, entity, metaData);
                candidates->push_back(PropertyCandidate(modelAction));
            }
        }
    }
}

void ScanPlanetTemplate::gatherSponsorEntities(vector<PropertyCandidate> *candidates,
                                               const SpaceWorldModel &spaceModel) const {
    vector<WorldModelAction> actions;
    auto newEntityAction = spaceModel.CreateAgent();
    actions.push_back(newEntityAction);

    MetaData metaData;
    metaData.SetValue("relationToPlayer", 10);
    WorldModelAction metaDataAction(WorldActionType::UPDATE, newEntityAction.GetEntity(), metaData);
    actions.push_back(move(metaDataAction));

    candidates->push_back(PropertyCandidate(actions, newEntityAction.GetEntity()));

    for (auto entity : spaceModel.GetEntities()) {
        if (entity->GetType() == "agent") {
            auto entityData = spaceModel.GetMetaData(entity->GetId());
            if (entityData.GetValue("relationToPlayer") >= 10) {
                WorldModelAction modelAction(WorldActionType::KEEP, entity);
                candidates->push_back(PropertyCandidate(modelAction));
            }
        }
    }
}

void ScanPlanetTemplate::gatherCivilizationEntities(vector<PropertyCandidate> *candidates,
                                                    const SpaceWorldModel &spaceModel) const {
    auto newEntityAction = spaceModel.CreateDeadCivilization();
    candidates->push_back(PropertyCandidate(newEntityAction));

    for (auto entity : spaceModel.GetEntities()) {
        if (entity->GetType() == "deadCivilization") {
            WorldModelAction modelAction(WorldActionType::KEEP, entity);
            candidates->push_back(PropertyCandidate(modelAction));
        }
    }
}









