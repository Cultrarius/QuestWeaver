//
// Created by michael on 06.10.15.
//

#pragma once

#include "../WorldModel.h"
#include "SpaceLocation.h"

class SpaceWorldModel : public WorldModel {
public:
    std::shared_ptr<SpaceLocation> CreateLocation() const;
};

