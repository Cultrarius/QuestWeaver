//
// Created by michael on 10.10.15.
//

#pragma once

#include "../Quest.h"
#include "../../World/Space/SpaceLocation.h"
#include "../../World/Space/SpaceAgent.h"

namespace weave {
    class ExploreRegionQuest : public Quest {
    public:
        static const std::string metaDataMarker;

        ExploreRegionQuest(const std::string &title, const std::string &description, const std::string &story,
                           ID locationId, ID sponsorId);

        std::string GetType() const override;

        QuestTickResult Tick(float delta, const WorldModel &worldModel) override;

    protected:

        /*
         * Can be used by subclasses for serialization purposes
         */
        cereal::NameValuePair<const ID &> getCerealLocation() const {
            return cereal::make_nvp("solarSystem", targetLocation);
        }

        /*
         * Can be used by subclasses for serialization purposes
         */
        cereal::NameValuePair<const ID &> getCerealSponsor() const {
            return cereal::make_nvp("sponsor", sponsor);
        }

    private:
        ID targetLocation;
        ID sponsor;

        ExploreRegionQuest(ID id, const std::string &title, const std::string &description,
                           const std::string &story, ID location, ID sponsor);

        friend class cereal::access;

        template<class Archive>
        void serialize(Archive &archive) {
            archive(getCerealId(), getCerealTitle(), getCerealDescription(), getCerealStory(),
                    getCerealLocation(), getCerealSponsor());
        }

        template<class Archive>
        static void load_and_construct(Archive &ar, cereal::construct<ExploreRegionQuest> &construct) {
            ID id;
            std::string title;
            std::string description;
            std::string story;
            ID location;
            ID sponsor;

            ar(id, title, description, story, location, sponsor);
            construct(id, title, description, story, location, sponsor);
        }
    };
}

CEREAL_REGISTER_TYPE(weave::ExploreRegionQuest);
