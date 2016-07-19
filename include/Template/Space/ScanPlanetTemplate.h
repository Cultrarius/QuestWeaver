//
// Created by michael on 24.03.16.
//

#pragma once

#include <Template/QuestTemplate.h>
#include <World/Space/SpaceWorldModel.h>
#include <World/Space/Planet.h>
#include <World/Space/DeadCivilization.h>

namespace weave {
    class ScanPlanetTemplate : public QuestTemplate {
    public:
        std::shared_ptr<Quest> ToQuest(const std::vector<QuestPropertyValue> &questPropertyValues,
                                       const std::string &questStory) const override;

        std::vector<PropertyCandidate> GetPropertyCandidates(const TemplateQuestProperty &property,
                                                             const WorldModel &worldModel) const override;

        ScanPlanetTemplate(std::string title,
                           std::vector<TemplateQuestProperty> properties,
                           std::vector<TemplateQuestDescription> descriptions,
                           FormatterType formatterType,
                           int rarity);

    protected:
        std::map<std::string, std::string> resolveAdditionalPlaceholders(
                QuestPropertyValue questPropertyValue) const noexcept override;

    private:
        std::string metaDataMarker = "scanQuestLock";
        std::string planetProperty = Planet::Type;
        std::string sponsorProperty = "sponsor";
        std::string deadCivProperty = DeadCivilization::Type;

        void gatherPlanetEntities(std::vector<PropertyCandidate> *candidates, const SpaceWorldModel &spaceModel) const;

        void gatherSponsorEntities(std::vector<PropertyCandidate> *candidates, const SpaceWorldModel &spaceModel) const;

        void gatherCivilizationEntities(std::vector<PropertyCandidate> *candidates,
                                        const SpaceWorldModel &spaceModel) const;
    };
}
