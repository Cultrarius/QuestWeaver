//
// Created by michael on 06.10.15.
//

#include "WorldModelAction.h"

using namespace weave;

WorldActionType WorldModelAction::GetActionType() const {
    return actionType;
}

std::shared_ptr<WorldEntity> WorldModelAction::GetEntity() const {
    return entity;
}

MetaData WorldModelAction::GetMetaData() const {
    return metaData;
}
