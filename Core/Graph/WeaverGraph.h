//
// Created by michael on 04.11.15.
//

#pragma once

#include <map>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include "Node.h"

namespace weave {
    class WeaverGraph {
    public:
        void createNodeGroup(const std::string &groupName, bool isMandatory);
        void addNode(const Node &node);

    private:
        std::unordered_map<ID, std::vector<Node>> nodes;
        std::unordered_map<std::string, std::vector<Node>> groups;
        std::unordered_set<std::string> mandatoryGroups;
    };
}
