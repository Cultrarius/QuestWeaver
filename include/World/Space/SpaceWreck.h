//
// Created by michael on 16.03.16.
//

#pragma once

#include "../WorldEntity.h"
#include "SolarSystem.h"

namespace weave {

    struct SpaceWreck : public WorldEntity {
        static const std::string Type;

        const float X;
        const float Y;
        const int Seed;
        const std::string Name;
        const std::shared_ptr<SolarSystem> HomeSystem;

        SpaceWreck();

        SpaceWreck(float x, float y, int seed, std::string name, std::shared_ptr<SolarSystem> solarSystem);

        std::string ToString() const noexcept override;

        std::string GetType() const noexcept override;

    private:
        SpaceWreck(ID id, float x, float y, int seed, std::string name, std::shared_ptr<SolarSystem> solarSystem);

        // serialization
        friend class cereal::access;

        template<class Archive>
        void serialize(Archive &archive) {
            archive(cereal::make_nvp("id", GetId()),
                    CEREAL_NVP(X),
                    CEREAL_NVP(Y),
                    CEREAL_NVP(Seed),
                    CEREAL_NVP(Name),
                    CEREAL_NVP(HomeSystem));
        }

        template<class Archive>
        static void load_and_construct(Archive &ar, cereal::construct<SpaceWreck> &construct) {
            ID id;
            float X;
            float Y;
            int Seed;
            std::string Name;
            std::shared_ptr<SolarSystem> HomeSystem;

            ar(cereal::make_nvp("id", id),
               CEREAL_NVP(X),
               CEREAL_NVP(Y),
               CEREAL_NVP(Seed),
               CEREAL_NVP(Name),
               CEREAL_NVP(HomeSystem));
            construct(id, X, Y, Seed, Name, HomeSystem);
        }
    };
}

CEREAL_REGISTER_TYPE(weave::SpaceWreck);
