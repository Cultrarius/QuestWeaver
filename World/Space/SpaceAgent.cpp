//
// Created by michael on 12.10.15.
//

#include <World/Space/SpaceAgent.h>

const std::string weave::SpaceAgent::Type = "agent";

weave::SpaceAgent::SpaceAgent(std::string name) : SpaceAgent(NoID, name) {
}

weave::SpaceAgent::SpaceAgent(ID id, std::string name) : WorldEntity(id), Name(name) {
}

std::string weave::SpaceAgent::ToString() const noexcept {
    return Name;
}

std::string weave::SpaceAgent::GetType() const noexcept {
    return Type;
}
