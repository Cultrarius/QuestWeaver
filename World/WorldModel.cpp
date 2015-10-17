//
// Created by michael on 10.08.15.
//

#include "WorldModel.h"

ID weave::WorldModel::NewId() {
    idGenerator++;
    return idGenerator;
}
