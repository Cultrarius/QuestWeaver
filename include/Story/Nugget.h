//
// Created by michael on 14.02.16.
//

#pragma once

#include <Core/WeaverTypes.h>

namespace weave {
    class Nugget {
    public:
        Nugget() = default;

        Nugget(std::string key, std::vector<std::string> requiredTypes, std::vector<std::string> texts);

        std::string GetKey();

        std::vector<std::string> GetRequiredTypes();

        std::vector<std::string> GetTexts();

    private:
        std::string key;
        std::vector<std::string> requiredTypes;
        std::vector<std::string> texts;
    };
}