//
// Created by michael on 10.08.15.
//

#pragma once

#include "cereal.h"
#include "../Core/WeaverTypes.h"
#include "../Core/WeaverUtils.h"
#include "WorldEntity.h"
#include "WorldModelAction.h"
#include "MetaData.h"
#include "WorldListener.h"

namespace weave {

    class WorldModel {
    public:
        explicit WorldModel(std::shared_ptr<RandomStream> rs);

        void Execute(std::vector<WorldModelAction> modelActions);

        std::shared_ptr<WorldEntity> GetEntityById(ID id) const;

        std::vector<std::shared_ptr<WorldEntity>> GetEntities() const;

        MetaData GetMetaData(ID entityId) const;

        std::vector<WorldModelAction> GetHistory() const;

        std::vector<WorldModelAction> GetMetaDataHistoryForId(ID id) const;

        void AddListener(std::shared_ptr<WorldListener> listener);

    protected:
        std::shared_ptr<RandomStream> rs;
        std::unordered_map<ID, MetaData> metaData;
        std::vector<WorldModelAction> actionHistory;

        ID NewId();

    private:
        ID idGenerator = 0;
        std::map<ID, std::shared_ptr<WorldEntity>> entities;
        std::vector<std::shared_ptr<WorldListener>> listeners;

        void updateMetaDataForId(ID newId, const MetaData &newData);

        bool hasEntityWithId(ID id) const;

        void informListeners(const std::vector<WorldModelAction> &actions);

        // serialization
        friend class cereal::access;

        template<class Archive>
        void serialize(Archive &archive) {
            archive(CEREAL_NVP(idGenerator), CEREAL_NVP(entities), CEREAL_NVP(rs), CEREAL_NVP(metaData),
                    CEREAL_NVP(actionHistory));
        }
    };
}
