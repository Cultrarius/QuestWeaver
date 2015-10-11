//
// Created by michael on 06.10.15.
//

#pragma once

#include "../WorldModel.h"
#include "SpaceLocation.h"
#include "../../Core/WeaverUtils.h"

namespace weave {
    class SpaceWorldModel : public WorldModel {
    public:
        explicit SpaceWorldModel(std::shared_ptr<RandomStream> randomStream);
        std::shared_ptr<SpaceLocation> CreateLocation() const;

    private:
        std::shared_ptr<RandomStream> rs;
    };
}
