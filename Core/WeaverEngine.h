//
// Created by michael on 10.08.15.
//

#pragma once

#include "../Template/Template.h"

namespace weave {
    class WeaverEngine {
    public:
        std::vector<QuestPropertyValue> fillTemplate(std::shared_ptr<Template> questTemplate,
                                                     const WorldModel &worldModel);
    };
}
