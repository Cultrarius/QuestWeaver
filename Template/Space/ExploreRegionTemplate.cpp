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
    std::shared_ptr<WorldEntity> newEntity;
    MetaData metaData;
    if (property.GetName() == "location") {
        newEntity = spaceModel.CreateLocation();
        metaData.SetValue("explored", 0);
        metaData.SetValue("explorationQuestLock", 1);  // so it does not get picked by another exploration quest

        for (auto entity : spaceModel.GetEntities()) {
            if (entity->GetType() == "location") {
                auto entityData = spaceModel.GetMetaData(entity->GetId());
                if (!entityData.HasValue("explorationQuestLock") && entityData.GetValue("explored") == 0) {
                    ModelAction modelAction(ActionType::UPDATE, entity, metaData);
                    actions.push_back(std::move(modelAction));
                }
            }
        }
    } else if (property.GetName() == "sponsor") {
        newEntity = spaceModel.CreateAgent();
    }
    ModelAction modelAction(ActionType::CREATE, newEntity, metaData);
    actions.push_back(std::move(modelAction));
    return actions;
}

shared_ptr<Quest> ExploreRegionTemplate::ToQuest(const vector<QuestPropertyValue> &questPropertyValues) const {
    const string &description = getBestFittingDescription(questPropertyValues);
    const string &title = getTitle(questPropertyValues);
    return make_shared<ExploreRegionQuest>(title, description);
}
