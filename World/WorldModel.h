//
// Created by michael on 10.08.15.
//

#pragma once

#include <memory>

namespace weave {

    class WorldModel {
    protected:
        uint64_t NewId();

    private:
        uint64_t idCounter = 0;
    };

}
