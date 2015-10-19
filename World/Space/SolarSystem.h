//
// Created by michael on 12.10.15.
//

#pragma once

#include "../WorldEntity.h"
#include "SpaceLocation.h"

namespace weave {

    struct SolarSystem : public WorldEntity {
        std::shared_ptr<SpaceLocation> Location;
        std::string Name;
        // TODO add planet infos?

        SolarSystem();

        SolarSystem(std::string Name, std::shared_ptr<SpaceLocation> Location);

        std::string ToString() const;

    private:
        SolarSystem(ID id, std::string Name, std::shared_ptr<SpaceLocation> Location);

        // serialization
        friend class cereal::access;

        template<class Archive>
        void serialize(Archive &archive) {
            archive(cereal::make_nvp("id", GetId()),
                    CEREAL_NVP(Name),
                    CEREAL_NVP(Location));
        }

        template<class Archive>
        static void load_and_construct(Archive &archive, cereal::construct<SolarSystem> &construct) {
            ID id;
            std::string Name;
            std::shared_ptr<SpaceLocation> Location;

            archive(cereal::make_nvp("id", id),
                    CEREAL_NVP(Name),
                    CEREAL_NVP(Location));
            construct(id, Name, Location);
        }
    };
}

CEREAL_REGISTER_TYPE(weave::SolarSystem);
