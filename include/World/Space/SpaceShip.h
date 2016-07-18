//
// Created by michael on 16.03.16.
//

#pragma once

#include "../WorldEntity.h"
#include "SpaceAgent.h"

namespace weave {

    struct SpaceShip : public WorldEntity {
        static const std::string Type;

        const int Seed;
        const std::string Name;
        const std::shared_ptr<SpaceAgent> Owner;

        SpaceShip();

        SpaceShip(int seed, std::string name, std::shared_ptr<SpaceAgent> owner);

        std::string ToString() const noexcept override;

        std::string GetType() const noexcept override;

    private:
        SpaceShip(ID id, int seed, std::string name, std::shared_ptr<SpaceAgent> owner);

        // serialization
        friend class cereal::access;

        template<class Archive>
        void serialize(Archive &archive) {
            archive(cereal::make_nvp("id", GetId()),
                    CEREAL_NVP(Seed),
                    CEREAL_NVP(Name),
                    CEREAL_NVP(Owner));
        }

        template<class Archive>
        static void load_and_construct(Archive &ar, cereal::construct<SpaceShip> &construct) {
            ID id;
            int Seed;
            std::string Name;
            std::shared_ptr<SpaceAgent> Owner;

            ar(cereal::make_nvp("id", id),
               CEREAL_NVP(Seed),
               CEREAL_NVP(Name),
               CEREAL_NVP(Owner));
            construct(id, Seed, Name, Owner);
        }
    };
}

CEREAL_REGISTER_TYPE(weave::SpaceShip);
