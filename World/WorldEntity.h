//
// Created by michael on 06.10.15.
//

#pragma once

#include "../Core/WeaverTypes.h"
#include "cereal.h"

namespace weave {

    class WorldEntity {
    public:
        static const ID NoID = 0;

        explicit WorldEntity(ID id) : id(id) { }

        WorldEntity() : WorldEntity(NoID) { }

        ID GetId();

        virtual std::string ToString() const = 0;

    private:
        ID id;

        // serialization
        friend class cereal::access;

        template<class Archive>
        void serialize(Archive &archive) {
            archive(CEREAL_NVP(id));
        }
    };
}


