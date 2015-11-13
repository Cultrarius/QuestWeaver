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
        action.Apply(graph);
        actionMap.clear();
    }

    unordered_map<string, Node> results;
    for (auto node : graph->GetActiveNodes()) {
        results[node.GetGroup()] = node;
    }
    return results;
}

bool GraphAction::operator==(const GraphAction &other) const {
    auto otherActivations = other.nodeActivations;
    for (auto pair : nodeActivations) {
        auto iter = otherActivations.find(pair.first);
        if (iter == otherActivations.end() || iter->second != pair.second) {
            return false;
        }
    }
    return true;
}

bool GraphAction::operator<(const GraphAction &other) const {
    auto otherActivations = other.nodeActivations;
    return (nodeActivations.size() < otherActivations.size());
}

bool GraphAnalyzer::fillActionMap(WeaverGraph *graph, map<GraphAction, float> *map, const AnalyzerParameters &param) {
    float startScore = getGraphScore(graph, param);

    auto mandatoryGroups = graph->GetMandatoryGroups();
    for (string group : graph->GetGroups()) {
        bool isMandatory = mandatoryGroups.find(group) != mandatoryGroups.end();
        for (auto &node : graph->GetNodes(group)) {
            // this is not the best solution for performance, but has better readability
            WeaverGraph graphCopy = *graph;
            auto isActive = graph->IsNodeActive(node);

            GraphAction action(!isActive, node);
            if (isMandatory && isActive) {
                // we can not deactivate nodes in mandatory groups
                continue;
            }
            action.Apply(&graphCopy);
            float actionScore = getGraphScore(&graphCopy, param);

            if (actionScore > startScore) {
                (*map)[action] = actionScore;
            }
        }
    }

    return !map->empty();
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

GraphAction::GraphAction(bool isActivate, const Node &node) {
    nodeActivations[node] = isActivate;
}

void GraphAction::Apply(WeaverGraph *graph) const {
    for (auto pair : nodeActivations) {
        if (pair.second) {
            graph->ActivateNode(pair.first);
        } else {
            graph->DeactivateNode(pair.first);
        }
    }
}
