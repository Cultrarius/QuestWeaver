//
// Created by michael on 04.11.15.
//

#include "WeaverGraph.h"

using namespace std;
using namespace weave;

void WeaverGraph::addNode(const Node &node) {
    if (nodes.find(node.GetId()) != nodes.end()) {
        throw ContractFailedException("Node with the given id already exists in graph.");
    }
    nodes[node.GetId()] = node;
    /*if (groups.find(node.GetGroup()) == groups.end()) {
        groups[node.GetGroup()] = vector<Node>();
    }*/
    groups[node.GetGroup()].push_back(node);
}
