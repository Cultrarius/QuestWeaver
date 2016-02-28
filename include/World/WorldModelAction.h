//
// Created by michael on 06.10.15.
//

#pragma once

#include <memory>
#include "cereal.h"
#include "WorldEntity.h"
#include "MetaData.h"


namespace weave {

    /*!
     * The different types of possible world model changes.
     *
     * @ingroup worldApi
     */
    enum class WorldActionType {
        //
                KEEP, /*!< Keep the entity in the model without changing it */
                CREATE, /*!< Register the entity with the model (requirement for any other action types) */
                DELETE, /*!< Remove the entity from the model */
                UPDATE /*!< Change the entity in the model */
    };

    /*!
     * Contains change information for a WorldModel entity.
     *
     * @ingroup worldApi
     */
    class WorldModelAction {
    public:

        /*!
         * Creates an **invalid** default action.
         */
        WorldModelAction() :
                actionType(WorldActionType::KEEP) { }

        /*!
         * @param actionType The type of change produced by this action
         * @param entity The entity being changed in the world model
         */
        WorldModelAction(const WorldActionType &actionType,
                         const std::shared_ptr<WorldEntity> entity) :
                actionType(actionType), entity(entity) { }

        /*!
         * @param actionType The type of change produced by this action
         * @param entity The entity being changed in the world model
         * @param metaData The new metadata for the given entity. If the actionType is KEEP or DELETE, then this data
         * is not used by the world model.
         */
        WorldModelAction(const WorldActionType &actionType,
                         const std::shared_ptr<WorldEntity> entity,
                         const MetaData metaData) :
                actionType(actionType), entity(entity), metaData(metaData) { }

        /*!
         * Returns the type of change produced by this action.
         */
        WorldActionType GetActionType() const;

        /*!
         * Returns the entity being changed in the world model.
         */
        std::shared_ptr<WorldEntity> GetEntity() const;

        /*!
         * Returns the new metadata for the given entity. If the actionType is KEEP or DELETE, then this data
         * is not used by the world model.
         */
        MetaData GetMetaData() const;

    private:
        WorldActionType actionType;
        std::shared_ptr<WorldEntity> entity;
        MetaData metaData;

        // serialization
        friend class cereal::access;

        template<class Archive>
        void serialize(Archive &archive) {
            archive(CEREAL_NVP(actionType), CEREAL_NVP(entity), CEREAL_NVP(metaData));
        }
    };
}
