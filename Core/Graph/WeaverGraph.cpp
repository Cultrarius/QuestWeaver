//
// Created by michael on 04.11.15.
//

#include "WeaverGraph.h"

using namespace std;
using namespace weave;

WeaverGraph &WeaverGraph::AddNode(const Node &node) {
    if (node.GetGroup() == "") {
        throw ContractFailedException("Can not add node with empty group!");
    }
    if (groups.find(node.GetGroup()) == groups.end()) {
        throw ContractFailedException("Unknown node group " + node.GetGroup());
    }
    nodes[node.GetId()].push_back(node);
    groups[node.GetGroup()].push_back(node);
    return *this;
}

WeaverGraph &WeaverGraph::CreateNodeGroup(const string &groupName, bool isMandatory) {
    if (groups.find(groupName) != groups.end()) {
        throw ContractFailedException("Group " + groupName + " already exists in graph");
    }
    groups[groupName] = vector<Node>();
    if (isMandatory) {
        mandatoryGroups.insert(groupName);
    }
    return *this;
}

WeaverGraph &WeaverGraph::AddEdge(Edge edge) {
    // check if the edge nodes are added to the graph
    auto nodeIt1 = nodes.find(edge.id1);
    auto nodeIt2 = nodes.find(edge.id2);
    if (nodeIt1 == nodes.end() || nodeIt2 == nodes.end()) {
        throw ContractFailedException("Can not create edge to unknown graph node!");
    }

    // check if the edge connects two nodes from the same node group. If so, silently drop it.
    unordered_set<string> nodeGroups;
    for (Node node : nodeIt1->second) {
        nodeGroups.insert(node.GetGroup());
    }
    for (Node node : nodeIt2->second) {
        if (nodeGroups.find(node.GetGroup()) != nodeGroups.end()) {
            return *this;
        }
    }

    // add the edge to the graph
    auto entry = edges.find(edge);
    if (entry == edges.end()) {
        edges.insert(edge);
    } else {
        edge.addTypesFrom(*entry);
        edges.erase(entry);
        edges.insert(edge);
    }
    return *this;
}

const std::set<Edge> &WeaverGraph::GetEdges() const {
    return edges;
}

std::vector<std::string> WeaverGraph::GetGroups() const {
    vector<string> groupNames;
    for (auto group : groups) {
        groupNames.push_back(group.first);
    }
    return groupNames;
}

std::vector<std::string> WeaverGraph::GetMandatoryGroups() const {
    vector<string> groupNames;
    for (auto group : mandatoryGroups) {
        groupNames.push_back(group);
    }
    return groupNames;
}

const std::vector<Node> &WeaverGraph::GetNodes(const std::string &groupName) const {
    auto iter = groups.find(groupName);
    if (iter == groups.end()) {
        throw ContractFailedException("Unable to get nodes for unknown group name!");
    }
    return iter->second;
}

void WeaverGraph::activateNode(const Node &node) {
    auto iter = groups.find(node.GetGroup());
    if (iter == groups.end()) {
        throw ContractFailedException("Unable to activate node from unknown group!");
    }
    for (Node groupNode : iter->second) {
        activeNodes.erase(groupNode);
    }
    activeNodes.insert(node);
}

bool WeaverGraph::deactivateNode(const Node &node) {
    auto iter = groups.find(node.GetGroup());
    if (iter == groups.end()) {
        throw ContractFailedException("Unable to deactivate node from unknown group!");
    }
    if (mandatoryGroups.find(node.GetGroup()) != mandatoryGroups.end()) {
        return false;
    }
    activeNodes.erase(node);
    return true;
}
