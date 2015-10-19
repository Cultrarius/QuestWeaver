//
// Created by michael on 12.10.15.
//

#pragma once

#include "../WorldEntity.h"
#include "SpaceLocation.h"

namespace weave {

    struct SolarSystem : public WorldEntity {
        SpaceLocation Location;
        std::string Name;
        // TODO add planet infos?

        SolarSystem();

        SolarSystem(std::string Name, SpaceLocation Location);

        std::string ToString() const;

    private:
        SolarSystem(ID id, std::string Name, SpaceLocation Location);

        //serialization
        friend class cereal::access;

        template<class Archive>
        void serialize(Archive &archive) {
            archive(cereal::make_nvp("id", GetId()),
                    CEREAL_NVP(Name),
                    CEREAL_NVP(Location));
        }

        template<class Archive>
        static void load_and_construct(Archive &ar, cereal::construct<SpaceLocation> &construct) {
            ID id;
            std::string name;
            SpaceLocation location;

            ar(id, name, location);
            construct(id, name, location);
        }
    };
}
