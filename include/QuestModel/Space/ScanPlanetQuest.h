//
// Created by michael on 25.03.16.
//

#pragma once

#include "../Quest.h"

namespace weave {
    class ScanPlanetQuest : public Quest {
    public:
        static const std::string metaDataMarker;

        ScanPlanetQuest(const std::string &title, const std::string &description, const std::string &story,
                        ID planetId, ID sponsorId);

        std::string GetType() const override;

        QuestTickResult Tick(float delta, const WorldModel &worldModel) override;

    protected:

        /*
         * Can be used by subclasses for serialization purposes
         */
        cereal::NameValuePair<const ID &> getCerealLocation() const {
            return cereal::make_nvp("planet", targetPlanet);
        }

        /*
         * Can be used by subclasses for serialization purposes
         */
        cereal::NameValuePair<const ID &> getCerealSponsor() const {
            return cereal::make_nvp("sponsor", sponsor);
        }

    private:
        ID targetPlanet;
        ID sponsor;

        ScanPlanetQuest(ID id, const std::string &title, const std::string &description,
                        const std::string &story, ID planetId, ID sponsor);

        friend class cereal::access;

        template<class Archive>
        void serialize(Archive &archive) {
            archive(getCerealId(), getCerealTitle(), getCerealDescription(), getCerealStory(),
                    getCerealLocation(), getCerealSponsor());
        }

        template<class Archive>
        static void load_and_construct(Archive &ar, cereal::construct<ScanPlanetQuest> &construct) {
            ID id;
            std::string title;
            std::string description;
            std::string story;
            ID targetPlanet;
            ID sponsor;

            ar(id, title, description, story, targetPlanet, sponsor);
            construct(id, title, description, story, targetPlanet, sponsor);
        }
    };
}

CEREAL_REGISTER_TYPE(weave::ScanPlanetQuest);
