//
// Created by michael on 06.10.15.
//

#include "SpaceLocation.h"

using namespace weave;

std::string SpaceLocation::ToString() const {
    return "(Location)";
}

SpaceLocation::SpaceLocation(uint64_t id, double x, double y, double z) : WorldEntity(id), X(x), Y(y), Z(z) {
}

SpaceLocation::SpaceLocation(double x, double y, double z) : SpaceLocation(NoID, x, y, z) {
}
