//
// Created by michael on 12.10.15.
//

#include <World/Space/SolarSystem.h>

using namespace weave;
using namespace std;

const string SolarSystem::Type = "solarSystem";

string SolarSystem::ToString() const noexcept {
    return Name;
}

SolarSystem::SolarSystem(ID id, string name, int seed, shared_ptr<SpaceLocation> location,
                         vector<shared_ptr<Planet>> planets) :
        WorldEntity(id), Name(name), Seed(seed), Location(location), Planets(planets) {
}

SolarSystem::SolarSystem(string name, int seed, shared_ptr<SpaceLocation> location, vector<shared_ptr<Planet>> planets)
        : SolarSystem(NoID, name, seed, location, planets) {
}

SolarSystem::SolarSystem() :
        SolarSystem(NoID, "", 0, make_shared<SpaceLocation>(), vector<shared_ptr<Planet>>()) {
}

std::string SolarSystem::GetType() const noexcept {
    return Type;
}
