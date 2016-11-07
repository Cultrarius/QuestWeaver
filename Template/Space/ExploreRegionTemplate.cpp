//
// Created by michael on 14.08.15.
//

#include <Template/Space/ExploreRegionTemplate.h>
#include <QuestModel/Space/ExploreRegionQuest.h>
#include <World/Space/SolarSystem.h>
#include <World/Space/SpaceAgent.h>
#include <World/Space/MetaDataMarkers.h>

using namespace std;
using namespace weave;

ExploreRegionTemplate::ExploreRegionTemplate(string title,
                                             vector<TemplateQuestProperty> properties,
                                             vector<TemplateQuestDescription> descriptions,
                                             FormatterType formatterType, int rarity)
        : QuestTemplate(title, properties, descriptions, formatterType, rarity, "ExploreRegionTemplate") {
}

vector<PropertyCandidate> ExploreRegionTemplate::GetPropertyCandidates(const TemplateQuestProperty &property,
                                                                       const WorldModel &worldModel) const {
    vector<PropertyCandidate> candidates;
    const SpaceWorldModel &spaceModel = (const SpaceWorldModel &) worldModel;
    if (property.GetName() == "solarSystem") {
        gatherSolarSystemEntities(&candidates, spaceModel);
    } else if (property.GetName() == "sponsor") {
        gatherSponsorEntities(&candidates, spaceModel);
    }
    return candidates;
}

void ExploreRegionTemplate::gatherSponsorEntities(vector<PropertyCandidate> *candidates,
                                                  const SpaceWorldModel &spaceModel) const {
    vector<WorldModelAction> actions;
    auto newEntityAction = spaceModel.CreateAgent();
    actions.push_back(newEntityAction);

    actions.emplace_back(WorldActionType::UPDATE, newEntityAction.GetEntity(),
                         MetaData(MetaDataMarkers::RelationToPlayer, 50));

    candidates->emplace_back(actions, newEntityAction.GetEntity());

    for (auto entity : spaceModel.GetEntitiesWithType(SpaceAgent::Type)) {
        auto entityData = spaceModel.GetMetaData(entity->GetId());
        if (entityData.GetValue("relationToPlayer") >= 50) {
            WorldModelAction modelAction(WorldActionType::KEEP, entity);
            candidates->emplace_back(modelAction);
        }
    }
}

void ExploreRegionTemplate::gatherSolarSystemEntities(vector<PropertyCandidate> *candidates,
                                                      const SpaceWorldModel &spaceModel) const {
    string exploredPercent = ExploreRegionQuest::metaDataMarker;
    auto newEntityActions = spaceModel.CreateSolarSystem();
    vector<WorldModelAction> actions;
    shared_ptr<WorldEntity> newSolarSystem;
    for (auto action : newEntityActions) {
        if (action.GetEntity()->GetType() == SolarSystem::Type) {
            newSolarSystem = action.GetEntity();
        }
        actions.push_back(action);
        MetaData metaData(exploredPercent, 0);
        metaData.SetValue(metaDataMarker, 1);  // so it does not get picked by another exploration quest
        actions.emplace_back(WorldActionType::UPDATE, action.GetEntity(), metaData);
    }
    candidates->emplace_back(actions, newSolarSystem);

    // search for existing unexplored solar systems
    for (auto entity : spaceModel.GetEntitiesWithType(SolarSystem::Type)) {
        auto entityData = spaceModel.GetMetaData(entity->GetId());
        if (!entityData.HasValue(metaDataMarker) && entityData.GetValue(exploredPercent) == 0) {
            MetaData metaData(exploredPercent, 0);
            metaData.SetValue(metaDataMarker, 1);
            candidates->emplace_back(WorldModelAction(WorldActionType::UPDATE, entity, metaData));
        }
    }
}

shared_ptr<Quest> ExploreRegionTemplate::ToQuest(const vector<QuestPropertyValue> &questPropertyValues,
                                                 const std::string &questStory) const {
    const string &description = getBestFittingDescription(questPropertyValues);
    const string &questTitle = getTitle(questPropertyValues);

    ID solarSystem = getEntityIdFromProperty("solarSystem", questPropertyValues);
    ID sponsor = getEntityIdFromProperty("sponsor", questPropertyValues);
    return make_shared<ExploreRegionQuest>(questTitle, description, questStory, solarSystem, sponsor);
}

map<string, string> ExploreRegionTemplate::resolveAdditionalPlaceholders(QuestPropertyValue questValue) const noexcept {
    map<std::string, std::string> result;
    auto entity = questValue.GetValue();
    if (entity->GetType() == SolarSystem::Type) {
        auto system = dynamic_pointer_cast<SolarSystem>(entity);
        bool isMandatory = questValue.GetProperty().IsMandatory();
        result["location"] = QuestPropertyValue::GetValueString(system->Location, isMandatory, formatterType);
    }
    return result;
}
