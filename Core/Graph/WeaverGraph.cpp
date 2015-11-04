//
// Created by michael on 04.11.15.
//

#include "WeaverGraph.h"

using namespace std;
using namespace weave;

void WeaverGraph::addNode(const Node &node) {
    if (groups.find(node.GetGroup()) == groups.end()) {
        throw ContractFailedException("Unknown node group " + node.GetGroup());
    }
    nodes[node.GetId()].push_back(node);
    groups[node.GetGroup()].push_back(node);
}

void WeaverGraph::createNodeGroup(const string &groupName, bool isMandatory) {
    if (groups.find(groupName) != groups.end()) {
        throw ContractFailedException("Group " + groupName + " already exists in graph");
    }
    groups[groupName] = vector<Node>();
    if (isMandatory) {
        mandatoryGroups.insert(groupName);
    }
}
