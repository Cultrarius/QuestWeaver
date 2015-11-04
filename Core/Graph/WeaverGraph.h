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
        //void addGroup(const std::string &groupName, bool isMandatory);
        void addNode(const Node &node);

    private:
        //same entity in different groups??
        std::map<ID, Node> nodes;
        std::map<std::string, std::vector<Node>> groups;
    };
}
