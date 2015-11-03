//
// Created by michael on 12.10.15.
//

#pragma once

#include "../WorldEntity.h"

namespace weave {

    struct SpaceAgent : public WorldEntity {
        std::string Name;

        explicit SpaceAgent(std::string name);

        std::string ToString() const;

        std::string GetType() const;

    private:
        SpaceAgent(ID id, std::string name);

        // serialization
        friend class cereal::access;

        template<class Archive>
        void serialize(Archive &archive) {
            archive(cereal::make_nvp("id", GetId()), CEREAL_NVP(Name));
        }

        template<class Archive>
        static void load_and_construct(Archive &ar, cereal::construct<SpaceAgent> &construct) {
            ID id;
            std::string name;

            ar(id, name);
            construct(id, name);
        }
    };
}

CEREAL_REGISTER_TYPE(weave::SpaceAgent);
