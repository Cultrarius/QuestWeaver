//
// Created by michael on 06.10.15.
//

#pragma once

#include "WorldEntity.h"
#include <memory>

namespace weave {

    enum class ActionType {
        KEEP, CREATE, DELETE
    };

    class ModelAction {
    private:
        ActionType actionType;
        std::shared_ptr<WorldEntity> entity;

    public:
        ModelAction(const ActionType &actionType,
                    const std::shared_ptr<WorldEntity> entity) :
                actionType(actionType), entity(entity) { }

        ActionType GetActionType() const;

        std::shared_ptr<WorldEntity> GetEntity() const;
    };
}
