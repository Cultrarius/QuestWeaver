//
// Created by michael on 06.10.15.
//

#pragma once

#include "../Core/WeaverTypes.h"
#include "cereal.h"

namespace weave {

    /*!
     * Represents an object in the game that is relevant for the quest system.
     *
     * @ingroup worldApi
     */
    class WorldEntity {
    public:
        /*!
         * Entities not registered with any world model have this default ID.
         */
        static const ID NoID = 0;

        /*!
         * Creates a new entity without an ID.
         */
        WorldEntity() : WorldEntity(NoID) { }

		virtual ~WorldEntity() {}

        /*!
         * Returns the entity's ID.
         */
        ID GetId() const;

        /*!
         * Returns the type of the entity (e.g. "location" or "beast"). This type is used in the quest and story
         * templates to define which entities can be used for the quest generation.
         */
        virtual std::string GetType() const noexcept = 0;

        /*!
         * Returns a string representation of the entity to be used in the quest generation.
         */
        virtual std::string ToString() const noexcept = 0;

    protected:
        /*!
         * Creates a new entity with the given ID. This constructor should only be used by subclasses for
         * deserialization.
         */
        explicit WorldEntity(ID id) : id(id) { }

    private:
        friend class WorldModel;

        ID id;

        // serialization
        friend class cereal::access;

        template<class Archive>
        void serialize(Archive &archive) {
            archive(CEREAL_NVP(id));
        }
    };
}

