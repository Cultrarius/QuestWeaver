//
// Created by michael on 06.10.15.
//

#pragma once

#include <memory>
#include "cereal.h"
#include "WorldEntity.h"
#include "MetaData.h"


namespace weave {

    enum class ActionType {
        KEEP, CREATE, DELETE, UPDATE
    };

    class ModelAction {
    public:
        ModelAction() :
                actionType(ActionType::KEEP) { }

        ModelAction(const ActionType &actionType,
                    const std::shared_ptr<WorldEntity> entity) :
                actionType(actionType), entity(entity) { }

        ModelAction(const ActionType &actionType,
                    const std::shared_ptr<WorldEntity> entity,
                    const MetaData metaData) :
                actionType(actionType), entity(entity), metaData(metaData) { }

        ActionType GetActionType() const;

        std::shared_ptr<WorldEntity> GetEntity() const;

        MetaData GetMetaData() const;

    private:
        ActionType actionType;
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
