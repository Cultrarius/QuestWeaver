//
// Created by michael on 14.08.15.
//

#pragma once

#include "Template/QuestTemplate.h"
#include "../../World/Space/SpaceWorldModel.h"

namespace weave {
    class ExploreRegionTemplate : public QuestTemplate {
    public:
        std::shared_ptr<Quest> ToQuest(const std::vector<QuestPropertyValue> &questPropertyValues,
                                       const std::string &questStory) const override;

        std::vector<PropertyCandidate> GetPropertyCandidates(const TemplateQuestProperty &property,
                                                             const WorldModel &worldModel) const override;

        ExploreRegionTemplate(std::string title,
                              std::vector<TemplateQuestProperty> properties,
                              std::vector<TemplateQuestDescription> descriptions,
                              FormatterType formatterType,
                              int rarity);

    protected:
        std::map<std::string, std::string> resolveAdditionalPlaceholders(
                QuestPropertyValue questPropertyValue) const noexcept override;

    private:
        std::string metaDataMarker = "explorationQuestLock";

        void gatherSolarSystemEntities(std::vector<PropertyCandidate> *candidates,
                                       const weave::SpaceWorldModel &spaceModel) const;

        void gatherSponsorEntities(std::vector<PropertyCandidate> *candidates, const SpaceWorldModel &spaceModel) const;
    };
}

