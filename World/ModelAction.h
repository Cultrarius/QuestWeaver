//
// Created by michael on 06.10.15.
//

#pragma once

#include "WorldEntity.h"
#include "MetaData.h"
#include <memory>

namespace weave {

    enum class ActionType {
        KEEP, CREATE, DELETE, UPDATE
    };

    class ModelAction {
    private:
        ActionType actionType;
        std::shared_ptr<WorldEntity> entity;
        MetaData metaData;

    public:
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
    };
}
