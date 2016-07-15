//
// Created by michael on 16.03.16.
//

#include "World/Space/SpaceStation.h"

using namespace weave;
using namespace std;

const string SpaceStation::Type = "spaceStation";

std::string SpaceStation::ToString() const noexcept {
    return Name;
}

SpaceStation::SpaceStation(ID id, float x, float y, int seed, std::string name, shared_ptr<SolarSystem> system) :
        WorldEntity(id), X(x), Y(y), Seed(seed), Name(name), HomeSystem(system) {
}

SpaceStation::SpaceStation(float x, float y, int seed, std::string name, shared_ptr<SolarSystem> system) :
        SpaceStation(NoID, x, y, seed, name, system) {
}

SpaceStation::SpaceStation() : SpaceStation(0, 0, 0, 0, "", make_shared<SolarSystem>()) {
}

std::string SpaceStation::GetType() const noexcept {
    return Type;
}
