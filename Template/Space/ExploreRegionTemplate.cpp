//
// Created by michael on 14.08.15.
//

#include "ExploreRegionTemplate.h"
#include "../../World/Space/SpaceWorldModel.h"

ExploreRegionTemplate::ExploreRegionTemplate(std::vector<std::string> titles,
                                             std::vector<TemplateQuestProperty> properties,
                                             std::vector<TemplateQuestDescription> descriptions)
        : Template(titles, properties, descriptions) {

}

std::vector<ModelAction> ExploreRegionTemplate::GetPropertyCandidates(const TemplateQuestProperty &property,
                                                                      const WorldModel &worldModel) const {
    std::vector<ModelAction> properties;
    const SpaceWorldModel& spaceModel = (const SpaceWorldModel &) worldModel;
    const ModelAction modelAction = ModelAction(ActionType::CREATE, spaceModel.CreateLocation());
    properties.push_back(modelAction);
    return properties;
}

Quest ExploreRegionTemplate::ToQuest(std::vector<QuestPropertyValue> questPropertyValues) const {
    return Quest(Proposed, "Title", "Descr");
}
