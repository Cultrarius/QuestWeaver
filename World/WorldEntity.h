//
// Created by michael on 06.10.15.
//

#pragma once

#include <string>

namespace weave {

    class WorldEntity {
    public:
        virtual std::string toString() const = 0;
    };
}


