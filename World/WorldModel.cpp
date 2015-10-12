//
// Created by michael on 10.08.15.
//

#include "WorldModel.h"

uint64_t weave::WorldModel::NewId() {
 idCounter++;
 return idCounter;
}
