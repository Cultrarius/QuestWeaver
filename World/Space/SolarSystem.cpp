//
// Created by michael on 12.10.15.
//

#include "SolarSystem.h"

using namespace weave;
using namespace std;

string SolarSystem::ToString() const {
    return Name;
}

SolarSystem::SolarSystem(ID id, string Name, shared_ptr<SpaceLocation> Location) :
        WorldEntity(id), Name(Name), Location(Location) {
}

SolarSystem::SolarSystem(string Name, shared_ptr<SpaceLocation> Location) :
        SolarSystem(NoID, Name, Location) {
}

SolarSystem::SolarSystem() :
        SolarSystem(NoID, "", make_shared<SpaceLocation>()) {
}

std::string SolarSystem::GetType() const {
    return "solarSystem";
}
