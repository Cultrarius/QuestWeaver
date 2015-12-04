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

        WeaverGraph &AddShadowNode(ID shadowNodeId);

        void Finalize();

        void ActivateNode(const Node &node);

        bool DeactivateNode(const Node &node);

        const std::set<Edge> &GetEdges() const;

        std::vector<std::string> GetGroups() const;

        const std::unordered_set<std::string> GetMandatoryGroups() const;

        const std::vector<Node> &GetNodes(const std::string &groupName) const;

        const std::set<Node> &GetActiveNodes() const;

        bool IsNodeActive(const Node &node) const;

        const std::vector<Node> &GetNodesWithId(ID id) const;

    private:
        std::unordered_map<ID, std::vector<Node>> nodes;
        std::unordered_set<ID> shadowNodes;
        std::unordered_map<std::string, std::vector<Node>> groups;
        std::unordered_set<std::string> mandatoryGroups;
        std::set<Edge> edges;
        std::set<Node> activeNodes;
        std::vector<Node> empty;
        bool isFinalized = false;

        bool isShadowNode(ID shadowNodeId) const;

        void mergeAddEdge(Edge &edge);

        void addTransitiveEdges();

        void activateMandatoryGroups();

        void checkUnfinalized() const;
    };
}
