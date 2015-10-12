//
// Created by michael on 06.10.15.
//

#pragma once

#include <string>

namespace weave {

    class WorldEntity {
    public:
        const uint64_t NoID = 0;

        explicit WorldEntity(uint64_t id) : id(id) { }

        WorldEntity() : WorldEntity(NoID) { }

        uint64_t GetId();

        virtual std::string ToString() const = 0;

    private:
        uint64_t id;
    };
}


