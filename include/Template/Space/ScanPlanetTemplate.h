//
// Created by michael on 24.03.16.
//

#pragma once

#include "Template/QuestTemplate.h"
#include "../../World/Space/SpaceWorldModel.h"

namespace weave {
    class ScanPlanetTemplate : public QuestTemplate {
    public:
        std::shared_ptr<Quest> ToQuest(const std::vector<QuestPropertyValue> &questPropertyValues,
                                       const std::string &questStory) const override;

        std::vector<WorldModelAction> GetPropertyCandidates(const TemplateQuestProperty &property,
                                                            const WorldModel &worldModel) const override;

        ScanPlanetTemplate(std::string title,
                           std::vector<TemplateQuestProperty> properties,
                           std::vector<TemplateQuestDescription> descriptions,
                           FormatterType formatterType);

    protected:
        std::map<std::string, std::string> resolveAdditionalPlaceholders(
                QuestPropertyValue questPropertyValue) const noexcept;

    private:
        std::string metaDataMarker = "scanQuestLock";

        void gatherPlanetEntities(std::vector<WorldModelAction> *actions,
                                  const weave::SpaceWorldModel &spaceModel) const;

        void gatherSponsorEntities(std::vector<WorldModelAction> *actions, const SpaceWorldModel &spaceModel) const;
    };
}