//
// Created by michael on 03.05.16.
//

#pragma once

#include "../WorldEntity.h"

namespace weave {

    struct Artifact : public WorldEntity {
        static const std::string Type;

        const std::string Name;

        Artifact();

        explicit Artifact(std::string name);

        std::string ToString() const noexcept override;

        std::string GetType() const noexcept override;

    private:
        Artifact(ID id, std::string name);

        // serialization
        friend class cereal::access;

        template<class Archive>
        void serialize(Archive &archive) {
            archive(cereal::make_nvp("id", GetId()), CEREAL_NVP(Name));
        }

        template<class Archive>
        static void load_and_construct(Archive &ar, cereal::construct<Artifact> &construct) {
            ID id;
            std::string Name;

            ar(cereal::make_nvp("id", id),
               CEREAL_NVP(Name));
            construct(id, Name);
        }
    };
}

CEREAL_REGISTER_TYPE(weave::Artifact);
