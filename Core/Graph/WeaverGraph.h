//
// Created by michael on 04.11.15.
//

#pragma once

#include <map>
#include <vector>
#include <set>
#include <unordered_set>
#include <unordered_map>
#include "Node.h"
#include "Edge.h"

namespace weave {
    class WeaverGraph {
    public:
        WeaverGraph &CreateNodeGroup(const std::string &groupName, bool isMandatory);

        WeaverGraph &AddNode(const Node &node);

        WeaverGraph &AddEdge(Edge edge);

        const std::set<Edge> &GetEdges();

    private:
        std::unordered_map<ID, std::vector<Node>> nodes;
        std::unordered_map<std::string, std::vector<Node>> groups;
        std::unordered_set<std::string> mandatoryGroups;
        std::set<Edge> edges;
    };
}
