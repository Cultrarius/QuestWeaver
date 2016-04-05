//
// Created by michael on 16.03.16.
//

#pragma once

#include "../WorldEntity.h"
#include "SpaceLocation.h"

namespace weave {

    struct Planet : public WorldEntity {
        static const std::string Type;

        const float X;
        const float Y;
        const int Seed;
        const std::string Name;
        const std::shared_ptr<SpaceLocation> Location;

        Planet();

        Planet(float x, float y, int seed, std::string name, std::shared_ptr<SpaceLocation> location);

        std::string ToString() const noexcept override;

        std::string GetType() const noexcept override;

    private:
        Planet(ID id, float x, float y, int seed, std::string name, std::shared_ptr<SpaceLocation> location);

        // serialization
        friend class cereal::access;

        template<class Archive>
        void serialize(Archive &archive) {
            archive(cereal::make_nvp("id", GetId()),
                    CEREAL_NVP(X),
                    CEREAL_NVP(Y),
                    CEREAL_NVP(Seed),
                    CEREAL_NVP(Name),
                    CEREAL_NVP(Location));
        }

        template<class Archive>
        static void load_and_construct(Archive &ar, cereal::construct<Planet> &construct) {
            ID id;
            float X;
            float Y;
            int Seed;
            std::string Name;
            std::shared_ptr<SpaceLocation> Location;

            ar(cereal::make_nvp("id", id),
               CEREAL_NVP(X),
               CEREAL_NVP(Y),
               CEREAL_NVP(Seed),
               CEREAL_NVP(Name),
               CEREAL_NVP(Location));
            construct(id, X, Y, Seed, Name, Location);
        }
    };
}

CEREAL_REGISTER_TYPE(weave::Planet);
