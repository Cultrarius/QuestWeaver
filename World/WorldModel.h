//
// Created by michael on 10.08.15.
//

#pragma once

#include "cereal.h"
#include "../Core/WeaverTypes.h"
#include "../Core/WeaverUtils.h"
#include "WorldEntity.h"

namespace weave {

    class WorldModel {
    public:
        explicit WorldModel(std::shared_ptr<RandomStream> rs);

    protected:
        ID NewId();

        std::shared_ptr<RandomStream> rs;

    private:
        ID idGenerator = 0;
        std::vector<std::shared_ptr<WorldEntity>> entities;

        // serialization
        friend class cereal::access;

        template<class Archive>
        void serialize(Archive &archive) {
            archive(CEREAL_NVP(idGenerator), CEREAL_NVP(entities));
        }
    };
}
