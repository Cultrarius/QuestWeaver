//
// Created by michael on 07.12.15.
//

#pragma once


#include "WorldModelAction.h"

namespace weave {
    class WorldListener {
    public:
        virtual void WorldChanged(std::vector<WorldModelAction> modelActions) = 0;
    };
}