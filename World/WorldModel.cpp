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

void WorldModel::Execute(vector<WorldModelAction> modelActions) {
    for (auto action : modelActions) {
        ID id = action.GetEntity()->GetId();
        auto entity = GetEntityById(id);
        if (entity.get() == nullptr && action.GetActionType() != WorldActionType::CREATE) {
            throw ContractFailedException(
                    "Unable to execute model action: entity with id " + to_string(id) + " not found.");
        }
        if (action.GetActionType() == WorldActionType::KEEP) {
            // doing nothing
        } else if (action.GetActionType() == WorldActionType::CREATE) {
            if (id != WorldEntity::NoID) {
                throw ContractFailedException(
                        "Unable to execute model action create: entity already has an ID: " + to_string(id));
            }

            ID newId = NewId();
            action.GetEntity()->id = newId;
            entities[newId] = action.GetEntity();
            updateMetaDataForId(newId, action.GetMetaData());
        } else if (action.GetActionType() == WorldActionType::DELETE) {
            ID oldId = action.GetEntity()->GetId();
            action.GetEntity()->id = WorldEntity::NoID;
            entities.erase(oldId);
            metaData.erase(oldId);
        } else if (action.GetActionType() == WorldActionType::UPDATE) {
            if (id == WorldEntity::NoID) {
                throw ContractFailedException(
                        "Unable to execute model action update: entity is not registered with world model.");
            }
            updateMetaDataForId(id, action.GetMetaData());
        } else {
            throw ContractFailedException("Illegal action type.");
        }

        // update change history
        if (action.GetActionType() != WorldActionType::KEEP) {
            actionHistory.push_back(action);
        }
    }
    informListeners(modelActions);
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

std::vector<WorldModelAction> WorldModel::GetMetaDataHistoryForId(ID id) const {
    vector<WorldModelAction> result;
    for (auto action : actionHistory) {
        if (action.GetEntity()->GetId() == id && action.GetMetaData().GetValueNames().size() > 0) {
            result.push_back(action);
        }
    }
    return result;
}

std::vector<WorldModelAction> WorldModel::GetHistory() const {
    return actionHistory;
}

void WorldModel::AddListener(std::shared_ptr<WorldListener> listener) const {
    listeners.push_back(listener);
}

void WorldModel::informListeners(const std::vector<WorldModelAction> &actions) {
    for (auto &listener : listeners) {
        listener->WorldChanged(actions);
    }
}
