//
// Created by michael on 12.10.15.
//

#include "SolarSystem.h"

std::string weave::SolarSystem::ToString() const {
    return Name;
}

weave::SolarSystem::SolarSystem(weave::ID id, std::string Name, weave::SpaceLocation Location) :
        WorldEntity(id), Name(Name), Location(Location) {
}

weave::SolarSystem::SolarSystem(std::string Name, weave::SpaceLocation Location) :
        SolarSystem(NoID, Name, Location) {
}

weave::SolarSystem::SolarSystem() :
        SolarSystem(NoID, "", weave::SpaceLocation()) {
}
