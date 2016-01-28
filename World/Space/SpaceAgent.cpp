//
// Created by michael on 12.10.15.
//

#include <World/Space/SpaceAgent.h>

weave::SpaceAgent::SpaceAgent(std::string name) : SpaceAgent(NoID, name) {
}

weave::SpaceAgent::SpaceAgent(ID id, std::string name) : WorldEntity(id), Name(name) {
}

std::string weave::SpaceAgent::ToString() const {
    return Name;
}

std::string weave::SpaceAgent::GetType() const {
    return "agent";
}
