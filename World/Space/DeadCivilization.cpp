//
// Created by michael on 22.03.16.
//

#include "World/Space/DeadCivilization.h"

using namespace weave;
using namespace std;

const string DeadCivilization::Type = "deadCivilization";
const string DeadCivilization::PlanetMarker = "deadCivPlanetRuins";

std::string DeadCivilization::ToString() const noexcept {
    return Name;
}

DeadCivilization::DeadCivilization(ID id, std::string name) : WorldEntity(id), Name(name) {
}

DeadCivilization::DeadCivilization(std::string name) : DeadCivilization(NoID, name) {
}

DeadCivilization::DeadCivilization() : DeadCivilization(0, "") {
}

std::string DeadCivilization::GetType() const noexcept {
    return Type;
}
