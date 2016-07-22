//
// Created by michael on 25.03.16.
//

#pragma once

#include "../Quest.h"

namespace weave {
    class HuntAndKillQuest : public Quest {
    public:
        HuntAndKillQuest(const std::string &title, const std::string &description, const std::string &story,
                         ID targetShipId, ID solarSystemId, ID sponsorId);

        std::string GetType() const override;

        QuestTickResult Tick(float delta, const WorldModel &worldModel) override;

    protected:

        cereal::NameValuePair<const ID &> getCerealTarget() const {
            return cereal::make_nvp("targetShip", targetShip);
        }

        cereal::NameValuePair<const ID &> getCerealSolarSystem() const {
            return cereal::make_nvp("solarSystem", solarSystem);
        }

        cereal::NameValuePair<const ID &> getCerealSponsor() const {
            return cereal::make_nvp("sponsor", sponsor);
        }

    private:
        static const int relationAdd = 10;
        ID targetShip;
        ID solarSystem;
        ID sponsor;

        HuntAndKillQuest(ID id, const std::string &title, const std::string &description,
                         const std::string &story, ID targetShipId, ID solarSystemId, ID sponsorId);

        friend class cereal::access;

        template<class Archive>
        void serialize(Archive &archive) {
            archive(getCerealId(), getCerealTitle(), getCerealDescription(), getCerealStory(),
                    getCerealTarget(), getCerealSolarSystem(), getCerealSponsor());
        }

        template<class Archive>
        static void load_and_construct(Archive &ar, cereal::construct<HuntAndKillQuest> &construct) {
            ID id;
            std::string title;
            std::string description;
            std::string story;
            ID targetShip;
            ID solarSystem;
            ID sponsor;

            ar(id, title, description, story, targetShip, solarSystem, sponsor);
            construct(id, title, description, story, targetShip, solarSystem, sponsor);
        }
    };
}

CEREAL_REGISTER_TYPE(weave::HuntAndKillQuest);
