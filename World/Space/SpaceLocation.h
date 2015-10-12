//
// Created by michael on 06.10.15.
//

#pragma once

#include "../WorldEntity.h"

namespace weave {

    struct SpaceLocation : public WorldEntity {
        int X;
        int Y;
        int Z;

        SpaceLocation(int x, int y, int z);

        SpaceLocation(uint64_t id, int x, int y, int z);

        std::string ToString() const;
    };
}


