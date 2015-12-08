//
// Created by michael on 08.12.15.
//

#pragma once


#include "../../World/WorldListener.h"

namespace weave {
    struct TestWorldListener : WorldListener {
        std::vector<WorldModelAction> calledActions;

        void WorldChanged(std::vector<WorldModelAction> modelActions);
    };
}
