//
// Created by michael on 06.10.15.
//

#include <World/WorldModelAction.h>

using namespace weave;

WorldActionType WorldModelAction::GetActionType() const noexcept {
    return actionType;
}

std::shared_ptr<WorldEntity> WorldModelAction::GetEntity() const noexcept {
    return entity;
}

MetaData WorldModelAction::GetMetaData() const noexcept {
    return metaData;
}
