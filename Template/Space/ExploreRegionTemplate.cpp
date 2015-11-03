//
// Created by michael on 14.08.15.
//

#include "ExploreRegionTemplate.h"
#include "../../World/Space/SpaceWorldModel.h"
#include "../../QuestModel/Space/ExploreRegionQuest.h"

using namespace std;
using namespace weave;

ExploreRegionTemplate::ExploreRegionTemplate(string title,
                                             vector<TemplateQuestProperty> properties,
                                             vector<TemplateQuestDescription> descriptions)
        : Template(title, properties, descriptions) {
}

vector<ModelAction> ExploreRegionTemplate::GetPropertyCandidates(const TemplateQuestProperty &property,
                                                                 const WorldModel &worldModel) const {
    vector<ModelAction> actions;
    const SpaceWorldModel &spaceModel = (const SpaceWorldModel &) worldModel;
    if (property.GetName() == "location") {
        gatherLocationEntities(&actions, spaceModel);
    } else if (property.GetName() == "sponsor") {
        gatherSponsorEntities(&actions, spaceModel);
    }
    return actions;
}

void ExploreRegionTemplate::gatherSponsorEntities(vector<ModelAction> *actions,
                                                  const SpaceWorldModel &spaceModel) const {
    shared_ptr<WorldEntity> newEntity = spaceModel.CreateAgent();
    MetaData metaData;
    metaData.SetValue("relationToPlayer", 10);
    ModelAction modelAction(ActionType::CREATE, newEntity, metaData);
    actions->push_back(move(modelAction));

    for (auto entity : spaceModel.GetEntities()) {
        if (entity->GetType() == "agent") {
            auto entityData = spaceModel.GetMetaData(entity->GetId());
            if (entityData.GetValue("relationToPlayer") >= 10) {
                ModelAction modelAction(ActionType::KEEP, entity);
                actions->push_back(move(modelAction));
            }
        }
    }
}

void ExploreRegionTemplate::gatherLocationEntities(vector<ModelAction> *actions,
                                                   const SpaceWorldModel &spaceModel) const {
    shared_ptr<WorldEntity> newEntity = spaceModel.CreateLocation();
    MetaData metaData;
    metaData.SetValue("explored", 0);
    metaData.SetValue("explorationQuestLock", 1);  // so it does not get picked by another exploration quest
    ModelAction modelAction(ActionType::CREATE, newEntity, metaData);
    actions->push_back(move(modelAction));

    for (auto entity : spaceModel.GetEntities()) {
        if (entity->GetType() == "location") {
            auto entityData = spaceModel.GetMetaData(entity->GetId());
            if (!entityData.HasValue("explorationQuestLock") && entityData.GetValue("explored") == 0) {
                ModelAction modelAction(ActionType::UPDATE, entity, metaData);
                actions->push_back(move(modelAction));
            }
        }
    }
}

shared_ptr<Quest> ExploreRegionTemplate::ToQuest(const vector<QuestPropertyValue> &questPropertyValues) const {
    const string &description = getBestFittingDescription(questPropertyValues);
    const string &title = getTitle(questPropertyValues);
    return make_shared<ExploreRegionQuest>(title, description);
}
