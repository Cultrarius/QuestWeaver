//
// Created by michael on 16.03.16.
//

#include "World/Space/Planet.h"

using namespace weave;
using namespace std;

std::string Planet::ToString() const noexcept {
    return Name;
}

Planet::Planet(ID id, float x, float y, int seed, std::string name, shared_ptr<SpaceLocation> location) :
        WorldEntity(id), X(x), Y(y), Seed(seed), Name(name), Location(location) {
}

Planet::Planet(float x, float y, int seed, std::string name, shared_ptr<SpaceLocation> location) :
        Planet(NoID, x, y, seed, name, location) {
}

Planet::Planet() : Planet(0, 0, 0, 0, "", make_shared<SpaceLocation>()) {
}

std::string Planet::GetType() const noexcept {
    return "planet";
}
