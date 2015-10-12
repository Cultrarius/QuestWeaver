//
// Created by michael on 12.10.15.
//

#pragma once

#include "../WorldEntity.h"

namespace weave {

    struct SpaceAgent : public WorldEntity {
        std::string Name;

        explicit SpaceAgent(std::string name);

        SpaceAgent(uint64_t id, std::string name);

        std::string ToString() const;
    };
}
