//
// Created by michael on 10.08.15.
//

#pragma once

#include <memory>
#include "../Core/WeaverTypes.h"

namespace weave {

    class WorldModel {
    protected:
        ID NewId();

    private:
        ID idGenerator = 0;
    };
}
