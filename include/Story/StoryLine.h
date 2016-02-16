//
// Created by michael on 14.02.16.
//

#pragma once

#include <Core/WeaverTypes.h>
#include "Nugget.h"

namespace weave {

    class NuggetOption {
    public:
        NuggetOption(std::string nuggetKey, std::vector<ID> entityIDs) noexcept :
                nuggetKey(nuggetKey), entityIDs(entityIDs) { }

        std::string GetNuggetKey() const noexcept {
            return nuggetKey;
        }

        std::vector<ID> GetEntityIDs() const noexcept {
            return entityIDs;
        }


    private:
        std::string nuggetKey;
        std::vector<ID> entityIDs;
    };

    class StoryLine {
    public:
        StoryLine(std::string prePart, std::vector<NuggetOption> nuggetOptions, std::string postPart) noexcept :
                prePart(prePart), nuggetOptions(nuggetOptions), postPart(postPart) { }

        std::string GetPrePart() const noexcept {
            return prePart;
        }

        std::vector<NuggetOption> GetNuggetOptions() const noexcept {
            return nuggetOptions;
        }

        std::string GetPostPart() const noexcept {
            return postPart;
        }

    private:
        std::string prePart;
        std::vector<NuggetOption> nuggetOptions;
        std::string postPart;
    };
}
