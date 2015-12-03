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
    auto iter = groups.find(node.GetGroup());
    if (iter == groups.end()) {
        throw ContractFailedException("Unknown node group " + node.GetGroup());
    }
    for (auto addedNode : iter->second) {
        if (node == addedNode) {
            throw ContractFailedException("Can not add the same node twice to graph!");
        }
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
    // if not, check if they are shadow nodes
    auto nodeIt1 = nodes.find(edge.id1);
    auto nodeIt2 = nodes.find(edge.id2);
    if (nodeIt1 == nodes.end() || nodeIt2 == nodes.end()) {
        if ((isShadowNode(edge.id1) && nodeIt2 != nodes.end()) ||
            (isShadowNode(edge.id2) && nodeIt1 != nodes.end())) {
            mergeAddEdge(edge);
            return *this;
        }
        if (isShadowNode(edge.id1) && isShadowNode(edge.id2)) {
            throw ContractFailedException("Can not add edge between two shadow nodes!");
        }
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
    mergeAddEdge(edge);
    return *this;
}

void WeaverGraph::mergeAddEdge(Edge &edge) {
    auto entry = edges.find(edge);
    if (entry == edges.end()) {
        edges.insert(edge);
    } else {
        edge.addTypesFrom(*entry);
        edges.erase(entry);
        edges.insert(edge);
    }
}

const set<Edge> &WeaverGraph::GetEdges() const {
    return edges;
}

vector<string> WeaverGraph::GetGroups() const {
    vector<string> groupNames;
    for (auto group : groups) {
        groupNames.push_back(group.first);
    }
    return groupNames;
}

const unordered_set<string> WeaverGraph::GetMandatoryGroups() const {
    return mandatoryGroups;
}

const vector<Node> &WeaverGraph::GetNodes(const string &groupName) const {
    auto iter = groups.find(groupName);
    if (iter == groups.end()) {
        throw ContractFailedException("Unable to get nodes for unknown group name!");
    }
    return iter->second;
}

void WeaverGraph::ActivateNode(const Node &node) {
    if (nodes.find(node.GetId()) == nodes.end()) {
        throw ContractFailedException("Unable to activate unknown node!");
    }
    auto iter = groups.find(node.GetGroup());
    if (iter == groups.end()) {
        throw ContractFailedException("Unable to activate node from unknown group!");
    }
    for (Node groupNode : iter->second) {
        activeNodes.erase(groupNode);
    }
    activeNodes.insert(node);
}

bool WeaverGraph::DeactivateNode(const Node &node) {
    if (nodes.find(node.GetId()) == nodes.end()) {
        throw ContractFailedException("Unable to deactivate unknown node!");
    }
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

const set<Node> &WeaverGraph::GetActiveNodes() const {
    return activeNodes;
}

bool WeaverGraph::IsNodeActive(const Node &node) const {
    return activeNodes.find(node) != activeNodes.end();
}

const vector<Node> &WeaverGraph::GetNodesWithId(ID id) const {
    auto iter = nodes.find(id);
    if (iter == nodes.end()) {
        return empty;
    } else {
        return iter->second;
    }
}

WeaverGraph &WeaverGraph::AddShadowNode(ID shadowNodeId) {
    if (isShadowNode(shadowNodeId)) {
        throw ContractFailedException("Shadow node with id " + to_string(shadowNodeId) + " already present in graph!");
    }
    shadowNodes.insert(shadowNodeId);
    return *this;
}

bool WeaverGraph::isShadowNode(ID shadowNodeId) const { return shadowNodes.find(shadowNodeId) != shadowNodes.end(); }

void WeaverGraph::Finalize() {
    // activate a node from each mandatory group, so the graph is in a consistent state
    for (auto group : mandatoryGroups) {
        auto nodes = GetNodes(group);
        if (nodes.size() > 0) {
            ActivateNode(nodes.at(0));
        } else {
            throw ContractFailedException("Missing nodes for mandatory group!");
        }
    }

    // calculate the transitive edges
    for (auto edge1 : edges) {
        ID shadowId, nodeId;
        if (isShadowNode(edge1.id1)) {
            shadowId = edge1.id1;
            nodeId = edge1.id2;
        } else if (isShadowNode(edge1.id1)) {
            shadowId = edge1.id2;
            nodeId = edge1.id1;
        } else {
            continue;
        }
        for (auto edge2 : edges) {
            if (edge2.id1 == shadowId && edge2.id2 != nodeId) {
                edges.insert(Edge(nodeId, edge2.id2, EdgeType::TRANSITIVE));
            } else if (edge2.id2 == shadowId && edge2.id1 != nodeId) {
                edges.insert(Edge(nodeId, edge2.id1, EdgeType::TRANSITIVE));
            }
        }
    }

    //TODO this has a bad performance - loop over the shadow nodes, not the edges
}
