//
// Created by michael on 18.07.16.
//

#include "World/Space/SpaceWreck.h"

using namespace weave;
using namespace std;

const string SpaceWreck::Type = "spaceWreck";

std::string SpaceWreck::ToString() const noexcept {
    return Name;
}

SpaceWreck::SpaceWreck(ID id, float x, float y, int seed, std::string name, shared_ptr<SolarSystem> system) :
        WorldEntity(id), X(x), Y(y), Seed(seed), Name(name), HomeSystem(system) {
}

SpaceWreck::SpaceWreck(float x, float y, int seed, std::string name, shared_ptr<SolarSystem> system) :
        SpaceWreck(NoID, x, y, seed, name, system) {
}

SpaceWreck::SpaceWreck() : SpaceWreck(0, 0, 0, 0, "", make_shared<SolarSystem>()) {
}

std::string SpaceWreck::GetType() const noexcept {
    return Type;
}
