//
// Created by michael on 14.02.16.
//

#pragma once

#include <Core/WeaverTypes.h>
#include "Nugget.h"

namespace weave {

    class StoryLine {
    public:
        StoryLine(std::string prePart, std::vector<std::string> nuggetKeys, std::string postPart) noexcept :
                prePart(prePart), nuggetKeys(nuggetKeys), postPart(postPart) { }

        std::string GetPrePart() const noexcept {
            return prePart;
        }

        std::vector<std::string> GetNuggetKeys() const noexcept {
            return nuggetKeys;
        }

        std::string GetPostPart() const noexcept {
            return postPart;
        }

    private:
        std::string prePart;
        std::vector<std::string> nuggetKeys;
        std::string postPart;
    };
}
