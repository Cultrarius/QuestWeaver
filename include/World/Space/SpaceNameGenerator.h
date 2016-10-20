//
// Created by michael on 15.03.16.
//

#pragma once

#include <Core/WeaverTypes.h>

namespace weave {
    class SpaceNameGenerator : public NameGenerator {
    public:
        virtual std::string CreateName(NameType nameType, std::shared_ptr<RandomStream> random) const override;
    };
}
