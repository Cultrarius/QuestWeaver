//
// Created by michael on 12.10.15.
//

#include <World/Space/SolarSystem.h>

using namespace weave;
using namespace std;

string SolarSystem::ToString() const noexcept {
    return Name;
}

SolarSystem::SolarSystem(ID id, string name, shared_ptr<SpaceLocation> location, vector<shared_ptr<Planet>> planets) :
        WorldEntity(id), Name(name), Location(location), Planets(planets) {
}

SolarSystem::SolarSystem(string name, shared_ptr<SpaceLocation> location, vector<shared_ptr<Planet>> planets) :
        SolarSystem(NoID, name, location, planets) {
}

SolarSystem::SolarSystem() :
        SolarSystem(NoID, "", make_shared<SpaceLocation>(), vector<shared_ptr<Planet>>()) {
}

std::string SolarSystem::GetType() const noexcept {
    return "solarSystem";
}
