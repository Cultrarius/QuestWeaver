//
// Created by michael on 18.07.16.
//

#include "World/Space/SpaceShip.h"

using namespace weave;
using namespace std;

const string SpaceShip::Type = "spaceShip";

std::string SpaceShip::ToString() const noexcept {
    return Name;
}

SpaceShip::SpaceShip(ID id, int seed, std::string name, shared_ptr<SpaceAgent> owner) :
        WorldEntity(id), Seed(seed), Name(name), Owner(owner) {
}

SpaceShip::SpaceShip(int seed, std::string name, shared_ptr<SpaceAgent> owner) :
        SpaceShip(NoID, seed, name, owner) {
}

SpaceShip::SpaceShip() : SpaceShip(0, 0, "", make_shared<SpaceAgent>()) {
}

std::string SpaceShip::GetType() const noexcept {
    return Type;
}
