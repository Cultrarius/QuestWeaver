//
// Created by michael on 12.10.15.
//

#pragma once

#include "../WorldEntity.h"
#include "SpaceLocation.h"
#include "Planet.h"

namespace weave {

    struct SolarSystem : public WorldEntity {
        std::string Name;
        std::shared_ptr<SpaceLocation> Location;
        std::vector<std::shared_ptr<Planet>> Planets;

        SolarSystem();

        SolarSystem(std::string Name, std::shared_ptr<SpaceLocation> Location,
                    std::vector<std::shared_ptr<Planet>> planets);

        std::string ToString() const override;

        std::string GetType() const override;

    private:
        SolarSystem(ID id, std::string name, std::shared_ptr<SpaceLocation> location,
                    std::vector<std::shared_ptr<Planet>> planets);

        // serialization
        friend class cereal::access;

        template<class Archive>
        void serialize(Archive &archive) {
            archive(cereal::make_nvp("id", GetId()),
                    CEREAL_NVP(Name),
                    CEREAL_NVP(Location),
                    CEREAL_NVP(Planets));
        }

        template<class Archive>
        static void load_and_construct(Archive &archive, cereal::construct<SolarSystem> &construct) {
            ID id;
            std::string Name;
            std::shared_ptr<SpaceLocation> Location;
            std::vector<std::shared_ptr<Planet>> Planets;

            archive(cereal::make_nvp("id", id),
                    CEREAL_NVP(Name),
                    CEREAL_NVP(Location),
                    CEREAL_NVP(Planets));
            construct(id, Name, Location, Planets);
        }
    };
}

CEREAL_REGISTER_TYPE(weave::SolarSystem);
