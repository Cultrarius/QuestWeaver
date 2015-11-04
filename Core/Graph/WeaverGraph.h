//
// Created by michael on 04.11.15.
//

#pragma once

#include <map>
#include <vector>
#include "Node.h"

namespace weave {
    class WeaverGraph {
    public:
        void addNode(const Node &node);

    private:
        std::map<ID, Node> nodes;
        std::map<std::string, std::vector<Node>> groups;
    };
}
