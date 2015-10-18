//
// Created by michael on 06.10.15.
//

#include "SpaceWorldModel.h"

using namespace std;
using namespace weave;

shared_ptr<SpaceLocation> SpaceWorldModel::CreateLocation() const {
    double x = rs->GetIntInRange(param.minLocation, param.maxLocation);
    double y = rs->GetIntInRange(param.minLocation, param.maxLocation);
    double z = rs->GetIntInRange(param.minLocation, param.maxLocation);
    return make_shared<SpaceLocation>(x, y, z);
}

SpaceWorldModel::SpaceWorldModel(std::shared_ptr<RandomStream> randomStream) : WorldModel(randomStream) {
}

ModelParameters SpaceWorldModel::GetParameters() {
    return param;
}

void SpaceWorldModel::SetParameters(ModelParameters parameters) {
    this->param = parameters;
}

std::shared_ptr<SpaceAgent> SpaceWorldModel::CreateAgent() const {
    return make_shared<SpaceAgent>("Peter Parker");
}
