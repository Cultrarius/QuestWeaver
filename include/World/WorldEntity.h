//
// Created by michael on 06.10.15.
//

#pragma once

#include "../Core/WeaverTypes.h"
#include "cereal.h"

namespace weave {

    /*!
     * @ingroup worldApi
     */
    class WorldEntity {
    public:
        static const ID NoID = 0;

        WorldEntity() : WorldEntity(NoID) { }

		virtual ~WorldEntity() {}

        ID GetId();

        virtual std::string GetType() const = 0;

        virtual std::string ToString() const = 0;

    protected:
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


