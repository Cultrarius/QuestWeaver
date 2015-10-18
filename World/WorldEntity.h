//
// Created by michael on 06.10.15.
//

#pragma once

#include <string>
#include "../Core/WeaverTypes.h"

namespace weave {

    class WorldEntity {
    public:
        static const ID NoID = 0;

        explicit WorldEntity(ID id) : id(id) { }

        WorldEntity() : WorldEntity(NoID) { }

        ID GetId();

        virtual std::string ToString() const = 0;

    private:
        ID id;
    };
}


