//
// Created by michael on 22.03.16.
//

#pragma once

#include "../WorldEntity.h"

namespace weave {

    struct DeadCivilization : public WorldEntity {
        static const std::string Type;
        static const std::string PlanetMarker;

        const std::string Name;

        DeadCivilization();

        explicit DeadCivilization(std::string name);

        std::string ToString() const noexcept override;

        std::string GetType() const noexcept override;

    private:
        DeadCivilization(ID id, std::string name);

        // serialization
        friend class cereal::access;

        template<class Archive>
        void serialize(Archive &archive) {
            archive(cereal::make_nvp("id", GetId()), CEREAL_NVP(Name));
        }

        template<class Archive>
        static void load_and_construct(Archive &ar, cereal::construct<DeadCivilization> &construct) {
            ID id;
            std::string Name;

            ar(cereal::make_nvp("id", id),
               CEREAL_NVP(Name));
            construct(id, Name);
        }
    };
}

CEREAL_REGISTER_TYPE(weave::DeadCivilization);
