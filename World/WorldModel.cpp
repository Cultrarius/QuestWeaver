//
// Created by michael on 10.08.15.
//

#include "WorldModel.h"

weave::ID weave::WorldModel::NewId() {
    idGenerator++;
    return idGenerator;
}

weave::WorldModel::WorldModel(std::shared_ptr<RandomStream> rs) : rs(rs) {
}
