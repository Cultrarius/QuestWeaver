//
// Created by michael on 12.10.15.
//

#pragma once

#include "../WorldEntity.h"
#include "SpaceLocation.h"

namespace weave {

    struct SolarSystem : public WorldEntity {
        SpaceLocation Location;
        std::string Name;
        // TODO add planet infos?

        std::string toString() const;
    };
}
