//
// Created by michael on 14.08.15.
//

#include "ExploreRegionTemplate.h"
#include "../../World/Space/SpaceWorldModel.h"

using namespace std;

ExploreRegionTemplate::ExploreRegionTemplate(vector<string> titles,
                                             vector<TemplateQuestProperty> properties,
                                             vector<TemplateQuestDescription> descriptions)
        : Template(titles, properties, descriptions) {
}

vector<ModelAction> ExploreRegionTemplate::GetPropertyCandidates(const TemplateQuestProperty &property,
                                                                 const WorldModel &worldModel) const {
    vector<ModelAction> properties;
    const SpaceWorldModel& spaceModel = (const SpaceWorldModel &) worldModel;
    const ModelAction modelAction = ModelAction(ActionType::CREATE, spaceModel.CreateLocation());
    properties.push_back(modelAction);
    return properties;
}

Quest ExploreRegionTemplate::ToQuest(vector<QuestPropertyValue> questPropertyValues) const {
    return Quest(Proposed, "Title", "Descr");
}
