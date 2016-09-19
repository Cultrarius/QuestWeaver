//
// Created by michael on 06.10.15.
//

#pragma once

#include <memory>
#include "WorldEntity.h"
#include "MetaData.h"


namespace weave {

#pragma push_macro("DELETE")
#undef DELETE

    /*!
     * The different types of possible world model changes.
     *
     * @ingroup worldApi
     */
    enum class WorldActionType {
        KEEP,
        CREATE,
        DELETE,
        UPDATE,
    };

#pragma pop_macro("DELETE")

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
        WorldActionType GetActionType() const noexcept;

        /*!
         * Returns the entity being changed in the world model.
         */
        std::shared_ptr<WorldEntity> GetEntity() const noexcept;

        /*!
         * Returns the new metadata for the given entity. If the actionType is KEEP or DELETE, then this data
         * is not used by the world model.
         */
        MetaData GetMetaData() const noexcept;

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
