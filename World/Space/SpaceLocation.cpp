//
// Created by michael on 06.10.15.
//

#include "SpaceLocation.h"

using namespace weave;
using namespace std;

std::string SpaceLocation::ToString() const {
    return "(" + to_string(X) + ", " + to_string(Y) + ", " + to_string(Z) + ")";
}

SpaceLocation::SpaceLocation(uint64_t id, int x, int y, int z) : WorldEntity(id), X(x), Y(y), Z(z) {
}

SpaceLocation::SpaceLocation(int x, int y, int z) : SpaceLocation(NoID, x, y, z) {
}

SpaceLocation::SpaceLocation() : SpaceLocation(0, 0, 0) {
}

std::string SpaceLocation::GetType() const {
    return "location";
}
