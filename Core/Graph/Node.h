//
// Created by michael on 04.11.15.
//

#pragma once

#include "../WeaverTypes.h"

namespace weave {
    class Node {
    public:
        Node();

        Node(const std::string &groupName, ID nodeId);

        std::string GetGroup() const;

        ID GetId() const;

        bool operator==(const Node &other) const;

        bool operator<(const Node &other) const;

    private:
        std::string group;
        ID id;
    };
}
