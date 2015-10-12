//
// Created by michael on 06.10.15.
//

#pragma once

#include "../WorldEntity.h"

namespace weave {

    struct SpaceLocation : public WorldEntity {
        double X;
        double Y;
        double Z;

        SpaceLocation(double x, double y, double z);

        SpaceLocation(uint64_t id, double x, double y, double z);

        std::string toString() const;
    };
}


