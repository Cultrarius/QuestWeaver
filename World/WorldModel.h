//
// Created by michael on 10.08.15.
//

#pragma once

#include "cereal.h"
#include "../Core/WeaverTypes.h"
#include "../Core/WeaverUtils.h"
#include "WorldEntity.h"
#include "ModelAction.h"
#include "MetaData.h"

namespace weave {

    class WorldModel {
    public:
        explicit WorldModel(std::shared_ptr<RandomStream> rs);

        void Execute(std::vector<ModelAction> modelActions);

        std::shared_ptr<WorldEntity> GetEntityById(ID id) const;

        std::vector<std::shared_ptr<WorldEntity>> GetEntities() const;

        MetaData &GetMetaData(ID entityId);

    protected:
        std::shared_ptr<RandomStream> rs;
        std::map<ID, MetaData> metaData;

        ID NewId();

    private:
        ID idGenerator = 0;
        std::vector<std::shared_ptr<WorldEntity>> entities;

        // serialization
        friend class cereal::access;

        template<class Archive>
        void serialize(Archive &archive) {
            archive(CEREAL_NVP(idGenerator), CEREAL_NVP(entities), CEREAL_NVP(rs), CEREAL_NVP(metaData));
        }
    };
}
