//
// Created by michael on 16.03.16.
//

#pragma once

#include "../WorldEntity.h"

namespace weave {

    struct Planet : public WorldEntity {
        const float X;
        const float Y;
        const int Seed;
        const std::string Name;

        Planet();

        Planet(float x, float y, int seed, std::string name);

        std::string ToString() const override;

        std::string GetType() const override;

    private:
        Planet(ID id, float x, float y, int seed, std::string name);

        // serialization
        friend class cereal::access;

        template<class Archive>
        void serialize(Archive &archive) {
            archive(cereal::make_nvp("id", GetId()),
                    CEREAL_NVP(X),
                    CEREAL_NVP(Y),
                    CEREAL_NVP(Seed),
                    CEREAL_NVP(Name));
        }

        template<class Archive>
        static void load_and_construct(Archive &ar, cereal::construct<Planet> &construct) {
            ID id;
            float X;
            float Y;
            int Seed;
            std::string Name;

            ar(cereal::make_nvp("id", id),
               CEREAL_NVP(X),
               CEREAL_NVP(Y),
               CEREAL_NVP(Seed),
               CEREAL_NVP(Name));
            construct(id, X, Y, Seed, Name);
        }
    };
}

CEREAL_REGISTER_TYPE(weave::Planet);
