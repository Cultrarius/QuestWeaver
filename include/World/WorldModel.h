//
// Created by michael on 10.08.15.
//

#pragma once

#include "../cereal.h"
#include "../Core/WeaverTypes.h"
#include "../Core/WeaverUtils.h"
#include "WorldEntity.h"
#include "WorldModelAction.h"
#include "MetaData.h"
#include "WorldListener.h"

namespace weave {

    /*!
     * Holds information about all entities relevant for quests and quest generation.
     * The WorldModel is provided by the caller of the API, so it can create game-specific quest entities (e.g cowboys
     * for a wild-west game and aliens for a sci-fi game).
     *
     * WorldEntity instances are immutable and stateless, so the world model also manages the entities MetaData.
     * In addition, the world model keeps track of all changes ever made to any entities or meta data to create richer
     * quests and back-stories.
     *
     * @ingroup worldApi
     */
    class WorldModel {
    public:

        /*!
         * Creates a new empty world model.
         */
        WorldModel() noexcept;

        virtual ~WorldModel() = default;

        /*!
         * Commits the given changes to the world model and informs the attached WorldLister.
         * This method is the only way to change the data of the world model and subclasses should not break that
         * contract.
         *
         * If one of the given actions is illegal (e.g. unknown entity ID), a ContractFailedException will be thrown.
         * The actions executed up until that point will stay executed, the rest of the actions will not be executed.
         *
         * @param modelActions The actions to be executed. The actions are executed in order.
         */
        void Execute(std::vector<WorldModelAction> modelActions);

        /*!
         * Returns the entity for a given ID, or a nullptr if no such entity is known.
         */
        std::shared_ptr<WorldEntity> GetEntityById(ID id) const noexcept;

        /*!
         * Returns all entities with a given type.
         */
        std::vector<std::shared_ptr<WorldEntity>> GetEntitiesWithType(std::string type) const noexcept;

        /*!
         * Returns a list of all entities known to this world model.
         */
        std::vector<std::shared_ptr<WorldEntity>> GetEntities() const noexcept;

        /*!
         * Returns the metadata for the entity with the given ID. If no entity with that ID is known, an empty metadata
         * object is returned.
         */
        MetaData GetMetaData(ID entityId) const noexcept;

        /*!
         * Returns the action history of all actions executed by this world model.
         * If this history is executed on an empty world model, it should result in a copy of the current model.
         */
        std::vector<WorldModelAction> GetHistory() const noexcept;

        /*!
         * Returns a list of all actions that changed the metadata of the entity with the given ID.
         * If an action changed the entity itself (e.g. delete it), but not the metadata, it will not be returned.
         */
        std::vector<WorldModelAction> GetMetaDataHistoryForId(ID id) const noexcept;

        /*!
         * Adds a listener to the world model. The listener will receive all updates of the world model's Execute
         * method.
         */
        void AddListener(std::shared_ptr<WorldListener> listener) const noexcept;

        /*!
         * The name generator used to create the names of world entities and story elements.
         */
        virtual const NameGenerator &GetNameGenerator() const = 0;

        /*!
         * Creates an action that when executed by the world model changes the metadata of the entitiy with the given ID
         * to the value provided by the updater function parameter.
         */
        WorldModelAction ChangeMetaData(ID entityId, std::string key, std::function<int(int)> updater) const;

    protected:
        friend class QuestWeaver;

        /*!
         * The random stream can be used by subclasses to create randomized entities.
         */
        std::shared_ptr<RandomStream> rs;

    private:
        ID idGenerator = 0;
        std::map<ID, std::shared_ptr<WorldEntity>> entities;
        mutable std::vector<std::shared_ptr<WorldListener>> listeners;
        std::unordered_map<ID, MetaData> metaData;
        std::vector<WorldModelAction> actionHistory;
        mutable std::unordered_map<std::string, std::vector<std::shared_ptr<WorldEntity>>> entityTypeCache;

        ID NewId();

        void updateMetaDataForId(ID newId, const MetaData &newData);

        bool hasEntityWithId(ID id) const noexcept;

        void informListeners(const std::vector<WorldModelAction> &actions) const;

        // serialization
        friend class cereal::access;

        template<class Archive>
        void serialize(Archive &archive) {
            archive(CEREAL_NVP(idGenerator), CEREAL_NVP(entities), CEREAL_NVP(rs), CEREAL_NVP(metaData),
                    CEREAL_NVP(actionHistory));
        }
    };
}
