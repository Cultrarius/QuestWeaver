//
// Created by michael on 06.10.15.
//

#pragma once

#include <memory>
#include "cereal.h"
#include "WorldEntity.h"
#include "MetaData.h"


namespace weave {

    enum class WorldActionType {
        KEEP, CREATE, DELETE, UPDATE
    };

    class WorldModelAction {
    public:
        WorldModelAction() :
                actionType(WorldActionType::KEEP) { }

        WorldModelAction(const WorldActionType &actionType,
                         const std::shared_ptr<WorldEntity> entity) :
                actionType(actionType), entity(entity) { }

        WorldModelAction(const WorldActionType &actionType,
                         const std::shared_ptr<WorldEntity> entity,
                         const MetaData metaData) :
                actionType(actionType), entity(entity), metaData(metaData) { }

        WorldActionType GetActionType() const;

        std::shared_ptr<WorldEntity> GetEntity() const;

        MetaData GetMetaData() const;

    private:
        WorldActionType actionType;
        std::shared_ptr<WorldEntity> entity;
        MetaData metaData;

        // serialization
        friend class cereal::access;

        template<class Archive>
        void serialize(Archive &archive) {
            archive(CEREAL_NVP(actionType), CEREAL_NVP(entity), CEREAL_NVP(metaData));
        }
    };
}
