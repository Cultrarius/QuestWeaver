//
// Created by michael on 06.10.15.
//

#pragma once

#include "../WorldEntity.h"

namespace weave {

    struct SpaceLocation : public WorldEntity {
        const int X;
        const int Y;
        const int Z;

        SpaceLocation();

        SpaceLocation(int x, int y, int z);

        std::string ToString() const;

    private:
        SpaceLocation(ID id, int x, int y, int z);

        // serialization
        friend class cereal::access;

        template<class Archive>
        void serialize(Archive &archive) {
            archive(cereal::make_nvp("id", GetId()),
                    CEREAL_NVP(X),
                    CEREAL_NVP(Y),
                    CEREAL_NVP(Z));
        }

        template<class Archive>
        static void load_and_construct(Archive &ar, cereal::construct<SpaceLocation> &construct) {
            ID id;
            int X;
            int Y;
            int Z;

            ar(cereal::make_nvp("id", id),
               CEREAL_NVP(X),
               CEREAL_NVP(Y),
               CEREAL_NVP(Z));
            construct(id, X, Y, Z);
        }
    };
}

CEREAL_REGISTER_TYPE(weave::SpaceLocation);
