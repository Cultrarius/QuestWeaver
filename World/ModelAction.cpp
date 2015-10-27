//
// Created by michael on 06.10.15.
//

#include "ModelAction.h"

using namespace weave;

ActionType ModelAction::GetActionType() const {
    return actionType;
}

std::shared_ptr<WorldEntity> ModelAction::GetEntity() const {
    return entity;
}

MetaData ModelAction::GetMetaData() const {
    return metaData;
}
