//
// Created by michael on 06.10.15.
//

#include "ModelAction.h"

ActionType ModelAction::GetActionType() const {
    return actionType;
}

std::shared_ptr<WorldEntity> ModelAction::GetEntity() const {
    return entity;
}
