//
// Created by michael on 11.10.15.
//

#pragma once

#include "cereal.h"
#include "WeaverTypes.h"


namespace weave {

    bool replace(std::string *str, const std::string &from, const std::string &to);

    void replaceAll(std::string *str, const std::string &from, const std::string &to);
}
