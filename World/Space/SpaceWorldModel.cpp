//
// Created by michael on 06.10.15.
//

#include "SpaceWorldModel.h"

using namespace std;
using namespace weave;

shared_ptr<SpaceLocation> SpaceWorldModel::CreateLocation() const {
    return make_shared<SpaceLocation>();
}
