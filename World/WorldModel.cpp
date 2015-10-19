//
// Created by michael on 10.08.15.
//

#include "WorldModel.h"

using namespace weave;
using namespace std;

ID WorldModel::NewId() {
    idGenerator++;
    return idGenerator;
}

WorldModel::WorldModel(shared_ptr<RandomStream> rs) : rs(rs) {
}

void WorldModel::Execute(vector<ModelAction> modelActions) {
    for (auto action : modelActions) {
        ID id = action.GetEntity()->GetId();
        auto entity = GetEntityById(id);
        if (action.GetActionType() == ActionType::KEEP) {
            if (entity.get() == nullptr) {
                throw ContractFailedException(
                        "Unable to execute model action keep: entity with id " + to_string(id) + " not found.");
            }
        } else if (action.GetActionType() == ActionType::CREATE) {
            if (entity.get() != nullptr) {
                throw ContractFailedException(
                        "Unable to execute model action create: entity with id " + to_string(id) + " already exists.");
            }
            if (id != WorldEntity::NoID) {
                throw ContractFailedException(
                        "Unable to execute model action create: entity already has an ID: " + to_string(id));
            }

            action.GetEntity()->id = NewId();
            entities.push_back(action.GetEntity());
        } else if (action.GetActionType() == ActionType::DELETE) {
            if (entity.get() == nullptr) {
                throw ContractFailedException(
                        "Unable to execute model action delete: entity with id " + to_string(id) + " not found.");
            }
            // TODO delete
        }
    }
}

shared_ptr<WorldEntity> WorldModel::GetEntityById(ID id) const {
    for (auto entity : entities) {
        if (entity->GetId() == id) {
            return entity;
        }
    }
    return shared_ptr<WorldEntity>();
}
