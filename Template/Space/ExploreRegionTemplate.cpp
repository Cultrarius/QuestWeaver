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
    vector<ModelAction> properties;
    const SpaceWorldModel &spaceModel = (const SpaceWorldModel &) worldModel;
    std::shared_ptr<WorldEntity> entity;
    if (property.GetName() == "location") {
        entity = spaceModel.CreateLocation();
    } else if (property.GetName() == "sponsor") {
        entity = spaceModel.CreateAgent();
    }
    ModelAction modelAction(ActionType::CREATE, entity);
    properties.push_back(std::move(modelAction));
    return properties;
}

shared_ptr<Quest> ExploreRegionTemplate::ToQuest(const vector<QuestPropertyValue> &questPropertyValues) const {
    const string &description = getBestFittingDescription(questPropertyValues);
    const string &title = getTitle(questPropertyValues);
    return make_shared<ExploreRegionQuest>(title, description);
}
