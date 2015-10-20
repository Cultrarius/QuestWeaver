//
// Created by michael on 12.10.15.
//

#include "SpaceAgent.h"

weave::SpaceAgent::SpaceAgent(std::string name) : SpaceAgent(NoID, name) {
}

weave::SpaceAgent::SpaceAgent(ID id, std::string name) : WorldEntity(id), Name(name) {
}

std::string weave::SpaceAgent::ToString() const {
    return Name;
}

