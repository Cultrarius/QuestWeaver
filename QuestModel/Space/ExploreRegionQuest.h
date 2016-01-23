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
        ExploreRegionQuest(const std::string &title, const std::string &description, const std::string &story,
                           ID locationId, ID sponsorId);

        std::string GetType() const override;

    protected:
        auto getCerealLocation() const {
            return cereal::make_nvp("location", targetLocation);
        }

        auto getCerealSponsor() const {
            return cereal::make_nvp("sponsor", sponsor);
        }

    private:
        ID targetLocation;
        ID sponsor;

        ExploreRegionQuest(ID id, QuestState state, const std::string &title, const std::string &description,
                           const std::string &story, ID location, ID sponsor);

        std::shared_ptr<Quest> setStateAndId(ID newId, QuestState newState) const;

        friend class cereal::access;

        template<class Archive>
        void serialize(Archive &archive) {
            archive(getCerealId(), getCerealState(), getCerealTitle(), getCerealDescription(), getCerealStory(),
                    getCerealLocation(), getCerealSponsor());
        }

        template<class Archive>
        static void load_and_construct(Archive &ar, cereal::construct<ExploreRegionQuest> &construct) {
            ID id;
            QuestState state;
            std::string title;
            std::string description;
            std::string story;
            ID location;
            ID sponsor;

            ar(id, state, title, description, story, location, sponsor);
            construct(id, state, title, description, story, location, sponsor);
        }
    };
}

CEREAL_REGISTER_TYPE(weave::ExploreRegionQuest);
