//
// Created by michael on 08.12.15.
//

#include "TestWorldListener.h"

void weave::TestWorldListener::WorldChanged(std::vector<weave::WorldModelAction> modelActions) {
    for (auto action : modelActions) {
        calledActions.push_back(action);
    }
}
