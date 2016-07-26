//
// Created by michael on 04.11.15.
//

#include <Core/Graph/WeaverGraph.h>

using namespace std;
using namespace weave;

WeaverGraph &WeaverGraph::AddNode(const Node &node) {
    checkUnfinalized();
    if (node.GetGroup() == "") {
        Logger::Error(ContractFailedException("Can not add node with empty group!"));
        return *this;
    }
    auto iter = groups.find(node.GetGroup());
    if (iter == groups.end()) {
        Logger::Error(ContractFailedException("Unknown node group " + node.GetGroup()));
        return *this;
    }
    for (auto addedNode : iter->second) {
        if (node == addedNode) {
            Logger::Error(ContractFailedException("Can not add the same node twice to a graph!"));
            return *this;
        }
    }
    nodes[node.GetId()].push_back(node);
    groups[node.GetGroup()].push_back(node);
    return *this;
}

WeaverGraph &WeaverGraph::CreateNodeGroup(const string &groupName, bool isMandatory) {
    checkUnfinalized();
    if (groups.count(groupName) != 0) {
        Logger::Error(ContractFailedException("Group " + groupName + " already exists in graph"));
        return *this;
    }
    groups[groupName] = vector<Node>();
    if (isMandatory) {
        mandatoryGroups.insert(groupName);
    }
    return *this;
}

WeaverGraph &WeaverGraph::AddEdge(Edge edge) {
    checkUnfinalized();
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
            Logger::Error(ContractFailedException("Can not add edge between two shadow nodes!"));
        }
        Logger::Error(ContractFailedException("Can not create edge to unknown graph node!"));
    }

    // check if the edge connects two nodes from the same node group. If so, silently drop it.
    unordered_set<string> nodeGroups;
    for (Node node : nodeIt1->second) {
        nodeGroups.insert(node.GetGroup());
    }
    for (Node node : nodeIt2->second) {
        if (nodeGroups.count(node.GetGroup()) != 0) {
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
        edge.addQuestIdsFrom(*entry);
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
        auto ex = ContractFailedException("Unable to get nodes for unknown group name!");
        Logger::Fatal(ex);
        throw ex;
    }
    return iter->second;
}

void WeaverGraph::ActivateNode(const Node &node) {
    if (nodes.count(node.GetId()) == 0) {
        Logger::Error(ContractFailedException("Unable to activate unknown node!"));
        return;
    }
    auto iter = groups.find(node.GetGroup());
    if (iter == groups.end()) {
        Logger::Error(ContractFailedException("Unable to activate node from unknown group!"));
        return;
    }
    for (Node groupNode : iter->second) {
        activeNodes.erase(groupNode);
    }
    activeNodes.insert(node);
}

bool WeaverGraph::DeactivateNode(const Node &node) {
    if (nodes.count(node.GetId()) == 0) {
        Logger::Error(ContractFailedException("Unable to deactivate unknown node!"));
        return false;
    }
    if (groups.count(node.GetGroup()) == 0) {
        Logger::Error(ContractFailedException("Unable to deactivate node from unknown group!"));
        return false;
    }
    if (mandatoryGroups.count(node.GetGroup()) > 0) {
        return false;
    }
    activeNodes.erase(node);
    return true;
}

const set<Node> &WeaverGraph::GetActiveNodes() const {
    return activeNodes;
}

bool WeaverGraph::IsNodeActive(const Node &node) const {
    return activeNodes.count(node) > 0;
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
    checkUnfinalized();
    if (nodes.count(shadowNodeId) > 0) {
        Logger::Error(ContractFailedException("Cannot add shadow node with id " + to_string(shadowNodeId) +
                                              ", because a regular node with the same id already exists!"));
        return *this;
    }
    if (isShadowNode(shadowNodeId)) {
        Logger::Error(ContractFailedException(
                "Shadow node with id " + to_string(shadowNodeId) + " already present in graph!"));
    }
    shadowNodes.insert(shadowNodeId);
    return *this;
}

bool WeaverGraph::isShadowNode(ID shadowNodeId) const {
    return shadowNodes.count(shadowNodeId) > 0;
}

void WeaverGraph::Finalize() {
    checkUnfinalized();
    activateMandatoryGroups();
    addTransitiveEdges();
    isFinalized = true;
}

/**
 * activate a node from each mandatory group, so the graph is in a consistent state
 */
void WeaverGraph::activateMandatoryGroups() {
    for (auto group : mandatoryGroups) {
        auto nodes = GetNodes(group);
        if (nodes.size() == 0) {
            Logger::Error(ContractFailedException("Missing nodes for mandatory group!"));
            return;
        }
        bool hasActive = false;
        for (auto node : nodes) {
            if (IsNodeActive(node)) {
                hasActive = true;
                break;
            }
        }
        if (!hasActive) {
            ActivateNode(nodes.at(0));
        }
    }
}

void WeaverGraph::addTransitiveEdges() {
    for (ID shadowId : shadowNodes) {
        vector<Edge> nodeEdges;
        for (auto edge : edges) {
            if (edge.id1 == shadowId || edge.id2 == shadowId) {
                nodeEdges.push_back(edge);
            }
        }
        for (uint64_t i = 0; i < nodeEdges.size(); i++) {
            for (uint64_t k = i + 1; k < nodeEdges.size(); k++) {
                Edge edge1 = nodeEdges[i];
                Edge edge2 = nodeEdges[k];
                ID nodeId1 = edge1.id1 == shadowId ? edge1.id2 : edge1.id1;
                ID nodeId2 = edge2.id1 == shadowId ? edge2.id2 : edge2.id1;
                Edge newEdge(nodeId1, nodeId2, EdgeType::TRANSITIVE);
                mergeAddEdge(newEdge);
            }
        }
    }
}

void WeaverGraph::checkUnfinalized() const {
    if (isFinalized) {
        Logger::Error(ContractFailedException("Graph was already finalized"));
    }
}
