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

            ID newId = NewId();
            action.GetEntity()->id = newId;
            entities[newId] = action.GetEntity();
            updateMetaDataForId(newId, action.GetMetaData());
        } else if (action.GetActionType() == ActionType::DELETE) {
            if (entity.get() == nullptr) {
                throw ContractFailedException(
                        "Unable to execute model action delete: entity with id " + to_string(id) + " not found.");
            }
            ID oldId = action.GetEntity()->GetId();
            action.GetEntity()->id = WorldEntity::NoID;
            entities.erase(oldId);
            metaData.erase(oldId);
        } else {
            throw ContractFailedException("Illegal action type.");
        }
    }
}

void WorldModel::updateMetaDataForId(ID newId, const MetaData &newData) {
    for (string name : newData.GetValueNames()) {
        metaData[newId].SetValue(name, newData.GetValue(name));
    }
}

shared_ptr<WorldEntity> WorldModel::GetEntityById(ID id) const {
    return hasEntityWithId(id) ? entities.find(id)->second : shared_ptr<WorldEntity>();
}

bool WorldModel::hasEntityWithId(ID id) const {
    return id != 0 && entities.find(id) != entities.end();
}

vector<shared_ptr<WorldEntity>> WorldModel::GetEntities() const {
    vector<shared_ptr<WorldEntity>> result;
    for (auto pair : entities) {
        result.push_back(pair.second);
    }
    return result;
}

MetaData &WorldModel::GetMetaData(ID entityId) {
    return metaData[entityId];
}
