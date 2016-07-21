//
// Created by michael on 24.03.16.
//

#pragma once

#include <Template/QuestTemplate.h>
#include <World/Space/SpaceWorldModel.h>
#include <World/Space/SolarSystem.h>

namespace weave {
    class HuntAndKillTemplate : public QuestTemplate {
    public:
        std::shared_ptr<Quest> ToQuest(const std::vector<QuestPropertyValue> &questPropertyValues,
                                       const std::string &questStory) const override;

        std::vector<PropertyCandidate> GetPropertyCandidates(const TemplateQuestProperty &property,
                                                             const WorldModel &worldModel) const override;

        HuntAndKillTemplate(std::string title,
                            std::vector<TemplateQuestProperty> properties,
                            std::vector<TemplateQuestDescription> descriptions,
                            FormatterType formatterType,
                            int rarity);

    protected:
        std::map<std::string, std::string> resolveAdditionalPlaceholders(
                QuestPropertyValue questPropertyValue) const noexcept override;

    private:
        std::string metaDataMarker = "killQuestLock";
        std::string solarSystemProperty = SolarSystem::Type;
        std::string targetShipProperty = "targetShip";
        std::string sponsorProperty = "sponsor";

        void gatherSolarSystemEntities(std::vector<PropertyCandidate> *candidates,
                                       const SpaceWorldModel &spaceModel) const;

        void gatherTargetEntities(std::vector<PropertyCandidate> *candidates, const SpaceWorldModel &spaceModel) const;

        void gatherSponsorEntities(std::vector<PropertyCandidate> *candidates, const SpaceWorldModel &spaceModel) const;
    };
}
