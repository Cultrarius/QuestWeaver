//
// Created by michael on 07.11.15.
//

#include "GraphAnalyzer.h"

using namespace std;
using namespace weave;

unordered_map<string, Node> GraphAnalyzer::SolveGraph(weave::WeaverGraph *graph,
                                                      shared_ptr<RandomStream> rs,
                                                      AnalyzerParameters param) {
    // activate random mandatory nodes, so the graph is in a consistent state
    for (auto group : graph->GetMandatoryGroups()) {
        auto nodes = graph->GetNodes(group);
        int index = rs->GetRandomIndex(nodes.size());
        graph->ActivateNode(nodes[index]);
    }

    map<GraphAction, float> actionMap;
    while (fillActionMap(graph, &actionMap, param)) {
        auto action = actionMap.begin()->first;
        if (action.IsActivate()) {
            graph->ActivateNode(action.GetNode());
        } else {
            graph->DeactivateNode(action.GetNode());
        }
        actionMap.clear();
    }

    unordered_map<string, Node> results;
    for (auto node : graph->GetActiveNodes()) {
        results[node.GetGroup()] = node;
    }
    return results;
}

const Node &GraphAction::GetNode() const {
    return node;
}

bool GraphAction::IsActivate() const {
    return isActivate;
}

bool GraphAction::operator==(const GraphAction &other) const {
    return node == other.node && isActivate == other.isActivate;
}

bool GraphAction::operator<(const GraphAction &other) const {
    return node < other.node && isActivate == other.isActivate;
}

bool GraphAnalyzer::fillActionMap(WeaverGraph *graph, map<GraphAction, float> *map, const AnalyzerParameters &param) {
    float startScore = getGraphScore(graph, param);

    auto mandatoryGroups = graph->GetMandatoryGroups();
    for (string group : graph->GetGroups()) {
        bool isMandatory = mandatoryGroups.find(group) != mandatoryGroups.end();
        for (auto &node : graph->GetNodes(group)) {
            auto isActive = graph->IsNodeActive(node);
            bool hasActiveNode = isActive || graph->HasActiveNode(group);

            GraphAction action(!isActive, node);
            float actionScore = 0;
            if (isMandatory) {
                if (isActive) {
                    // we can not deactivate nodes in mandatory groups
                    continue;
                }
                actionScore = tryMandatoryNode(graph, group, node, param);
            } else {
                actionScore = tryOptionalNode(graph, group, node, isActive, hasActiveNode, param);
            }

            if (actionScore > startScore) {
                (*map)[action] = actionScore;
            }
        }
    }

    return !map->empty();
}

float GraphAnalyzer::tryOptionalNode(WeaverGraph *graph, const string &group, const Node &node, bool isActive,
                                     bool hasActiveNode, const AnalyzerParameters &param) {
    float actionScore = 0;
    if (isActive) {
        graph->DeactivateNode(node);
        actionScore = getGraphScore(graph, param);
        graph->ActivateNode(node);
    } else if (hasActiveNode) {
        auto originalActiveNode = graph->GetActiveNode(group);
        graph->ActivateNode(node);
        actionScore = getGraphScore(graph, param);
        graph->ActivateNode(originalActiveNode);
    } else {
        graph->ActivateNode(node);
        actionScore = getGraphScore(graph, param);
        graph->DeactivateNode(node);
    }
    return actionScore;
}

float GraphAnalyzer::tryMandatoryNode(WeaverGraph *graph, const string &group, const Node &node,
                                      const AnalyzerParameters &param) {
    auto originalActiveNode = graph->GetActiveNode(group);
    graph->ActivateNode(node);
    float actionScore = getGraphScore(graph, param);
    graph->ActivateNode(originalActiveNode);
    return actionScore;
}

float GraphAnalyzer::getGraphScore(WeaverGraph *graph, const AnalyzerParameters &param) {
    float score = 0;
    auto mandatory = graph->GetMandatoryGroups();
    unordered_set<ID> activeNodeIds;

    for (auto node : graph->GetActiveNodes()) {
        activeNodeIds.insert(node.GetId());
        if (mandatory.find(node.GetGroup()) == mandatory.end()) {
            score += param.optionalNodePenalty;
        }
    }
    for (auto edge : graph->GetEdges()) {
        if ((activeNodeIds.find(edge.GetNode1()) != activeNodeIds.end()) &&
            (activeNodeIds.find(edge.GetNode2()) != activeNodeIds.end())) {
            score += edge.Count(EdgeType::DIRECT) * param.previousQuestBonus;
            score += edge.Count(EdgeType::TRANSITIVE) * param.transitiveQuestBonus;
        }
    }
    return score;
}

std::unordered_map<std::string, Node> GraphAnalyzer::SolveGraph(WeaverGraph *graph,
                                                                std::shared_ptr<RandomStream> randomStream) {
    return SolveGraph(graph, randomStream, AnalyzerParameters());
}
