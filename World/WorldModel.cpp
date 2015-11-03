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
    actionHistory.reserve(10000);
}

void WorldModel::Execute(vector<ModelAction> modelActions) {
    for (auto action : modelActions) {
        ID id = action.GetEntity()->GetId();
        auto entity = GetEntityById(id);
        if (entity.get() == nullptr && action.GetActionType() != ActionType::CREATE) {
            throw ContractFailedException(
                    "Unable to execute model action: entity with id " + to_string(id) + " not found.");
        }
        if (action.GetActionType() == ActionType::KEEP) {
            // doing nothing
        } else if (action.GetActionType() == ActionType::CREATE) {
            if (id != WorldEntity::NoID) {
                throw ContractFailedException(
                        "Unable to execute model action create: entity already has an ID: " + to_string(id));
            }

            ID newId = NewId();
            action.GetEntity()->id = newId;
            entities[newId] = action.GetEntity();
            updateMetaDataForId(newId, action.GetMetaData());
        } else if (action.GetActionType() == ActionType::DELETE) {
            ID oldId = action.GetEntity()->GetId();
            action.GetEntity()->id = WorldEntity::NoID;
            entities.erase(oldId);
            metaData.erase(oldId);
        } else if (action.GetActionType() == ActionType::UPDATE) {
            if (id == WorldEntity::NoID) {
                throw ContractFailedException(
                        "Unable to execute model action update: entity is not registered with world model.");
            }
            updateMetaDataForId(id, action.GetMetaData());
        } else {
            throw ContractFailedException("Illegal action type.");
        }

        // update change history
        if (action.GetActionType() != ActionType::KEEP) {
            actionHistory.push_back(action);
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

MetaData WorldModel::GetMetaData(ID entityId) const {
    auto mapEntry = metaData.find(entityId);
    return mapEntry == metaData.end() ? MetaData() : mapEntry->second;
}
